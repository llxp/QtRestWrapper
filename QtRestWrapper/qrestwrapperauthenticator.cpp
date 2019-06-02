#include "qrestwrapperauthenticator.h"

#include <QMetaMethod>
#include <QRegularExpression>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineView>
#include <QApplication>
#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>

using QtRestWrapper::QRestWrapperAuthenticator;

QRestWrapperAuthenticator::QRestWrapperAuthenticator(QObject *parent)
    : QObject(parent),
      m_cookieJar(new QRestWrapperCookieJar(parent)),
      // set the this pointer to the url interceptor,
      // that it can read the filter lists
      // and invoke authenticationCheck signal
      m_urlInterceptor(nullptr),
      m_view(nullptr),  // will be instantiated later
      m_page(nullptr),  // will be instantiated later
      m_cookieStore(nullptr),  // will be assigned later
      m_isAuthenticated(false),
      m_alternateAuthenticationCheck(false),
      m_fetchContentOnAuthenticated(false),
      m_fetchContentOnAuthenticationCheck(false)
{
}

QRestWrapperAuthenticator::~QRestWrapperAuthenticator()
{
    /*if(m_view != nullptr) {
        m_view->stop();
        if(m_view->parentWidget() != nullptr) {
            m_view->parentWidget()->layout()->removeWidget(m_view);
        }
        setParent(nullptr);
        m_urlInterceptor->deleteLater();
        m_urlInterceptor = nullptr;
        m_cookieJar->deleteLater();
        m_cookieJar = nullptr;
        m_view->deleteLater();
        m_view = nullptr;
        m_page->deleteLater();
        m_page = nullptr;
    }*/
}

void QRestWrapperAuthenticator::authenticate()
{
    this->m_isAuthenticated = false;
    initWebEngineView();
    emit this->addWebView(m_view);
    if(m_view != nullptr) {
        this->m_view->show();
        m_view->setEnabled(false);
        this->m_view->setUrl(this->m_applicationUrl);
        m_view->setEnabled(true);
    }
}

void QRestWrapperAuthenticator::setStoragePath(const QString &path)
{
    m_storagePath = "";
    // check, if the string is an absolute path or relative path
    if(path.startsWith('/') ||
       path.startsWith("\\\\") ||
       // exclude the possibility, that the string begins with e.g. C:
       (path.length() >= 2 && path[1] == ':')) {
        // make it an absolute path, by adding the current application path
        // to the beginning of the string
        m_storagePath = path;
    } else {
        m_storagePath = qApp->applicationDirPath();
        if(!m_storagePath.endsWith('/')) {
            m_storagePath += '/';
        }
        m_storagePath += path;
    }
    m_storagePath = m_storagePath.replace('\\', '/');
    if(!m_storagePath.endsWith('/')) {
        m_storagePath += '/';
    }
}

void QRestWrapperAuthenticator::setAuthenticationTestUrl(const QUrl &url)
{
    m_authenticationTestUrl = url;
}

void QRestWrapperAuthenticator::setApplicationUrl(const QUrl &url)
{
    m_applicationUrl = url;
}

void QRestWrapperAuthenticator::setAlternateAuthenticationCheck(bool alternateAuthenticationCheck)
{
    m_alternateAuthenticationCheck = alternateAuthenticationCheck;
}

void QRestWrapperAuthenticator::setFetchContentOnAuthenticated(bool fetchContentOnAuthenticated)
{
    m_fetchContentOnAuthenticated = fetchContentOnAuthenticated;
}

void QRestWrapperAuthenticator::setFetchContentOnAuthenticationCheck(bool fetchContentOnAuthenticationCheck)
{
    m_fetchContentOnAuthenticationCheck = fetchContentOnAuthenticationCheck;
}

void QRestWrapperAuthenticator::checkUrlForAuthentication(const QUrl &url)
{
    // check if the 'QRestWrapper::authenticationCheck' signal is connected
    if(this->m_alternateAuthenticationCheck) {
        // the signal is connected
        checkUrlForCustomAuthentication(url);
    } else {
        // the signal is not connected
        checkUrlForImplementedAuthentication(url);
    }
}

void QRestWrapperAuthenticator::checkUrlForCustomAuthentication(const QUrl &url)
{
    if(m_fetchContentOnAuthenticationCheck) {
        auto connection = new QMetaObject::Connection();
        *connection = connect(this->m_page, &QWebEnginePage::loadFinished, [this, url, connection](bool ok) {
            disconnect(*connection);
            if(ok) {
                m_page->toHtml([this, url](const QString &html) {
                    if(!this->m_isAuthenticated && authenticationCheckContent(url, html.toUtf8())) {
                        urlCheckFinished(url);
                    }
                });
            } else {
                m_page->toHtml([this, url](const QString &html) {
                    if(!this->m_isAuthenticated && authenticationCheckContent(url, html.toUtf8())) {
                        urlCheckFinished(url);
                    }
                });
            }
        });
    } else {
        if(!this->m_isAuthenticated && this->authenticationCheck(url)) {
            urlCheckFinished(url);
        }
    }
}

void QRestWrapperAuthenticator::checkUrlForImplementedAuthentication(const QUrl &url)
{
    QRegularExpression re(this->m_authenticationTestUrl.toString(), QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch match = re.match(QUrl::fromPercentEncoding(url.toString().toUtf8()));
    if (!this->m_isAuthenticated && match.hasMatch()) {
        urlCheckFinished(url);
    }
}

void QRestWrapperAuthenticator::urlCheckFinished(const QUrl &url)
{
    this->m_isAuthenticated = true;
    m_view->setEnabled(false);
    disconnect(m_page, &QRestWrapperPage::certificateErrorSignal, this, &QRestWrapperAuthenticator::certificateError);
    disconnect(m_page, &QWebEnginePage::selectClientCertificate, this, &QRestWrapperAuthenticator::clientCertificateNeeded);
    disconnect(m_cookieStore, &QWebEngineCookieStore::cookieAdded, m_cookieJar, &QRestWrapperCookieJar::insertCookie);
    disconnect(m_cookieStore, &QWebEngineCookieStore::cookieRemoved, m_cookieJar, &QRestWrapperCookieJar::deleteCookie);
    disconnect(m_view, &QWebEngineView::urlChanged, this, &QRestWrapperAuthenticator::checkUrlForAuthentication);
    disconnect(m_page, &QRestWrapperPage::certificateErrorSignal, this, &QRestWrapperAuthenticator::certificateError);

    disconnect(this->m_urlInterceptor, &QRestWrapperUrlInterceptor::checkAuthenticationPage, this, &QRestWrapperAuthenticator::checkUrlForAuthentication);

    if(m_fetchContentOnAuthenticated) {
        auto connection = new QMetaObject::Connection();
        *connection = connect(this->m_page, &QWebEnginePage::loadFinished, [this, connection, url](bool ok) {
            disconnect(*connection);
            if(ok) {
                this->m_page->toHtml([this, url](const QString &html) {
                    m_view->stop();
                    if(m_view->parentWidget() != nullptr) {
                        m_view->parentWidget()->layout()->removeWidget(m_view);
                    }
                    setParent(nullptr);
                    m_urlInterceptor->deleteLater();
                    m_urlInterceptor = nullptr;
                    m_cookieJar->deleteLater();
                    m_cookieJar = nullptr;
                    m_view->deleteLater();
                    m_view = nullptr;
                    m_page->deleteLater();
                    m_page = nullptr;
                    emit this->authenticatedContent(url, html.toUtf8());
                });
            } else {
                m_view->stop();
                if(m_view->parentWidget() != nullptr) {
                    m_view->parentWidget()->layout()->removeWidget(m_view);
                }
                setParent(nullptr);
                m_urlInterceptor->deleteLater();
                m_urlInterceptor = nullptr;
                m_cookieJar->deleteLater();
                m_cookieJar = nullptr;
                m_view->deleteLater();
                m_view = nullptr;
                m_page->deleteLater();
                m_page = nullptr;
                emit this->authenticatedContent(url, "");
            }
        });
    } else {
        m_view->stop();
        qDebug() << "deleting page and profile.";
        if(m_view->parentWidget() != nullptr) {
            m_view->parentWidget()->layout()->removeWidget(m_view);
        }
        setParent(nullptr);
        m_urlInterceptor->deleteLater();
        m_urlInterceptor = nullptr;
        m_cookieJar->deleteLater();
        m_cookieJar = nullptr;
        m_view->deleteLater();
        m_view = nullptr;
        m_page->deleteLater();
        m_page = nullptr;
        qDebug() << "...";
        emit this->authenticated(url);
    }
}

void QRestWrapperAuthenticator::initWebEngineView()
{
    if(m_cookieJar != nullptr) {
        m_cookieJar->deleteLater();
    }
    m_cookieJar = new QRestWrapperCookieJar();
    m_cookieJar->clearAllCookies();
    if(m_view != nullptr) {
        m_view->deleteLater();
    }
    m_view = new QWebEngineView(nullptr);
    //m_view->setAttribute(Qt::WA_DeleteOnClose, true);

    m_urlInterceptor = new QRestWrapperUrlInterceptor(this);

    connect(m_urlInterceptor, &QRestWrapperUrlInterceptor::checkAuthenticationPage, this, &QRestWrapperAuthenticator::checkUrlForAuthentication);

    m_page = new QRestWrapperPage(m_view);
    m_page->setRequestInterceptor(m_urlInterceptor);

    if(m_storagePath.length() <= 0) {
        m_page->setCachePath(qApp->applicationDirPath() + "/webcache/cache");
        m_page->setPersistentStoragePath(qApp->applicationDirPath() + "/webcache/persistent");
    } else {
        m_page->setCachePath(m_storagePath + "webcache/cache");
        m_page->setPersistentStoragePath(m_storagePath + "webcache/persistent");
    }

    m_cookieStore = this->m_page->profile()->cookieStore();

    connect(m_cookieStore, &QWebEngineCookieStore::cookieAdded, m_cookieJar, &QRestWrapperCookieJar::insertCookie);
    connect(m_cookieStore, &QWebEngineCookieStore::cookieRemoved, m_cookieJar, &QRestWrapperCookieJar::deleteCookie);

    connect(m_view, &QWebEngineView::urlChanged, this, &QRestWrapperAuthenticator::checkUrlForAuthentication);
    connect(m_page, &QRestWrapperPage::certificateErrorSignal, this, &QRestWrapperAuthenticator::certificateError);

    m_view->setPage(m_page);

    connect(this->m_page, &QWebEnginePage::selectClientCertificate, this, &QRestWrapperAuthenticator::clientCertificateNeeded);
}

void QRestWrapperAuthenticator::clientCertificateNeeded(QWebEngineClientCertificateSelection clientCertificateSelection)
{
    int i = 0;
    QDialog *certSelectionDialog = new QDialog();
    certSelectionDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    QTableWidget *certList = new QTableWidget();
    certList->setRowCount(clientCertificateSelection.certificates().size());
    certList->setColumnCount(4);
    certList->setHorizontalHeaderLabels(QStringList({"issuer", "subject", "activation Date", "expiry Date"}));
    for(QSslCertificate cert : clientCertificateSelection.certificates()) {
        QString version = cert.version();
        QString issuerDisplayName = cert.issuerDisplayName();
        QString subjectDisplayName = cert.subjectDisplayName();
        QDateTime activationDate = cert.effectiveDate();
        QDateTime expiryDate = cert.expiryDate();
        QTableWidgetItem *issuerDisplayNameItem = new QTableWidgetItem(issuerDisplayName);
        issuerDisplayNameItem->setFlags(issuerDisplayNameItem->flags() ^ Qt::ItemIsEditable);
        certList->setItem(i, 0, issuerDisplayNameItem);
        QTableWidgetItem *subjectDisplayNameItem = new QTableWidgetItem(subjectDisplayName);
        subjectDisplayNameItem->setFlags(subjectDisplayNameItem->flags() ^ Qt::ItemIsEditable);
        certList->setItem(i, 1, subjectDisplayNameItem);
        QTableWidgetItem *activationDateItem = new QTableWidgetItem(activationDate.toString());
        activationDateItem->setFlags(activationDateItem->flags() ^ Qt::ItemIsEditable);
        certList->setItem(i, 2, activationDateItem);
        QTableWidgetItem *expiryDateItem = new QTableWidgetItem(expiryDate.toString());
        expiryDateItem->setFlags(expiryDateItem->flags() ^ Qt::ItemIsEditable);
        certList->setItem(i, 3, expiryDateItem);
        ++i;
    }
    QModelIndex selectedIndex;
    QPushButton *okButton = new QPushButton(certSelectionDialog);
    certList->connect(certList, &QTableWidget::clicked, [&selectedIndex, okButton](const QModelIndex &index) {
        selectedIndex = index;
        okButton->setDisabled(false);
    });
    certList->connect(certList, &QTableWidget::doubleClicked, [certSelectionDialog, &clientCertificateSelection](const QModelIndex &index) {
        clientCertificateSelection.select(clientCertificateSelection.certificates().at(index.row()));
        certSelectionDialog->close();
        certSelectionDialog->deleteLater();
    });
    certSelectionDialog->setLayout(new QVBoxLayout(certSelectionDialog));
    certSelectionDialog->layout()->addWidget(certList);
    okButton->connect(okButton, &QPushButton::clicked, [certSelectionDialog, &clientCertificateSelection, &selectedIndex]() {
        clientCertificateSelection.select(clientCertificateSelection.certificates().at(selectedIndex.row()));
        certSelectionDialog->close();
        certSelectionDialog->deleteLater();
    });
    okButton->setDisabled(true);
    okButton->setText("Ok");
    okButton->setFixedWidth(100);
    okButton->setFixedHeight(50);
    certSelectionDialog->connect(certSelectionDialog, &QDialog::accepted, [certSelectionDialog, &clientCertificateSelection, &selectedIndex]() {
        clientCertificateSelection.select(clientCertificateSelection.certificates().at(selectedIndex.row()));
        certSelectionDialog->deleteLater();
    });
    certSelectionDialog->connect(certSelectionDialog, &QDialog::rejected, [certSelectionDialog]() {
        certSelectionDialog->deleteLater();
    });
    certSelectionDialog->layout()->addWidget(okButton);
    certSelectionDialog->exec();
}

/*
 * cookie retrieval/deletion functions
 */

void QRestWrapperAuthenticator::deleteCookiesByName(const QString &cookieName)
{
    return m_cookieJar->deleteCookiesByName(cookieName);
}

void QRestWrapperAuthenticator::deleteCookiesByDomain(const QString &cookieDomain)
{
    return m_cookieJar->deleteCookiesByDomain(cookieDomain);
}

void QRestWrapperAuthenticator::deleteCookiesByPath(const QString &cookiePath)
{
    return m_cookieJar->deleteCookiesByPath(cookiePath);
}

void QRestWrapperAuthenticator::deleteCookiesByNameAndDomain(const QString &cookieName, const QString &cookieDomain)
{
    return m_cookieJar->deleteCookiesByNameAndDomain(cookieName, cookieDomain);
}

void QRestWrapperAuthenticator::deleteCookiesByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath)
{
    return m_cookieJar->deleteCookiesByNameAndDomainAndPath(cookieName, cookieDomain, cookiePath);
}

void QRestWrapperAuthenticator::clearAllCookies()
{
    return m_cookieJar->clearAllCookies();
}

QVector<QNetworkCookie> QRestWrapperAuthenticator::getCookiesByName(const QString &cookieName) const
{
    return m_cookieJar->getCookiesByName(cookieName);
}

QVector<QNetworkCookie> QRestWrapperAuthenticator::getCookiesByDomain(const QString &cookieDomain) const
{
    return m_cookieJar->getCookiesByDomain(cookieDomain);
}

QVector<QNetworkCookie> QRestWrapperAuthenticator::getCookiesByPath(const QString &cookiePath) const
{
    return m_cookieJar->getCookiesByPath(cookiePath);
}

QVector<QNetworkCookie> QRestWrapperAuthenticator::getCookiesByNameAndDomain(const QString &cookieName, const QString &cookieDomain) const
{
    return m_cookieJar->getCookiesByNameAndDomain(cookieName, cookieDomain);
}

QVector<QNetworkCookie> QRestWrapperAuthenticator::getCookiesByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath) const
{
    return m_cookieJar->getCookiesByNameAndDomainAndPath(cookieName, cookieDomain, cookiePath);
}

QVector<QNetworkCookie> QRestWrapperAuthenticator::getAllCookies() const
{
    return m_cookieJar->getAllCookies();
}

QVector<QString> QRestWrapperAuthenticator::getOriginalCookieStringsByName(const QString &cookieName) const
{
    return m_cookieJar->getOriginalCookieStringsByName(cookieName);
}

QVector<QString> QRestWrapperAuthenticator::getOriginalCookieStringsByDomain(const QString &cookieDomain) const
{
    return m_cookieJar->getOriginalCookieStringsByDomain(cookieDomain);
}

QVector<QString> QRestWrapperAuthenticator::getOriginalCookieStringsByPath(const QString &cookiePath) const
{
    return m_cookieJar->getOriginalCookieStringsByPath(cookiePath);
}

QVector<QString> QRestWrapperAuthenticator::getOriginalCookieStringsByNameAndDomain(const QString &cookieName, const QString &cookieDomain) const
{
    return m_cookieJar->getOriginalCookieStringsByNameAndDomain(cookieName, cookieDomain);
}

QVector<QString> QRestWrapperAuthenticator::getOriginalCookieStringsByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath) const
{
    return m_cookieJar->getOriginalCookieStringsByNameAndDomainAndPath(cookieName, cookieDomain, cookiePath);
}

QVector<QString> QRestWrapperAuthenticator::getAllCookieStrings() const
{
    return m_cookieJar->getAllCookieStrings();
}