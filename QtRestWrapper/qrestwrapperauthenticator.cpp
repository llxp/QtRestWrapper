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
      m_cookieJar(parent),
      // set the this pointer to the url interceptor,
      // that it can read the filter lists
      // and invoke authenticationCheck signal
      m_urlInterceptor(new QRestWrapperUrlInterceptor(this)),
      m_view(nullptr),  // will be instantiated later
      m_page(nullptr),  // will be instantiated later
      m_cookieStore(nullptr),  // will be assigned later
      m_isAuthenticated(false),
      m_alternateAuthenticationCheck(false)
{
}

void QRestWrapperAuthenticator::authenticate()
{
    this->m_isAuthenticated = false;
    initWebEngineView();
    emit this->addWebView(this->m_view.get());
    this->m_view->setUrl(this->m_applicationUrl);
    this->m_view->show();
    this->m_cookieStore = this->m_page->profile()->cookieStore();
}

void QRestWrapperAuthenticator::setStoragePath(const QString &path)
{
    m_storagePath = path;
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

void QRestWrapperAuthenticator::checkUrlForAuthentication(const QUrl &url)
{
    // check if the 'QRestWrapper::authenticationCheck' signal is connected
    if(this->isSignalConnected(QMetaMethod::fromSignal(&QRestWrapperAuthenticator::authenticationCheck))) {
        // the signal is connected
        if(!this->m_isAuthenticated && this->authenticationCheck(url)) {
            this->m_isAuthenticated = true;
            disconnect(this->m_view.get(), &QWebEngineView::urlChanged, this, &QRestWrapperAuthenticator::checkUrlForAuthentication);
            emit this->removeWebView(this->m_view.get());
            this->m_view->deleteLater();
            this->m_view.take();
            this->m_page->deleteLater();
            this->m_page = nullptr;
            emit this->authenticated(url);
        }
    } else {
        // the signal is not connected
        QRegularExpression re(this->m_authenticationTestUrl.toString(), QRegularExpression::DotMatchesEverythingOption);
        QRegularExpressionMatch match = re.match(QUrl::fromPercentEncoding(url.toString().toUtf8()));
        if (!this->m_isAuthenticated && match.hasMatch()) {
            this->m_isAuthenticated = true;
            disconnect(this->m_view.get(), &QWebEngineView::urlChanged, this, &QRestWrapperAuthenticator::checkUrlForAuthentication);
            emit this->removeWebView(this->m_view.get());
            this->m_view->deleteLater();
            this->m_view.take();
            this->m_page->deleteLater();
            this->m_page = nullptr;
            emit this->authenticated(url);
        }
    }
}

void QRestWrapperAuthenticator::initWebEngineView()
{
    this->m_view.reset(new QWebEngineView());
    this->m_view->setEnabled(true);
    QWebEngineProfile* defaultProfile = QWebEngineProfile::defaultProfile();
    connect(this->m_urlInterceptor.get(), &QRestWrapperUrlInterceptor::checkAuthenticationPage, this, &QRestWrapperAuthenticator::checkUrlForAuthentication);
    defaultProfile->setRequestInterceptor(this->m_urlInterceptor.get());
    defaultProfile->setPersistentCookiesPolicy(QWebEngineProfile::AllowPersistentCookies);
    this->m_page = new QWebEnginePage(defaultProfile, this->m_view.get());
    this->m_page->settings()->setAttribute(QWebEngineSettings::AutoLoadImages, false);
    this->m_page->settings()->setAttribute(QWebEngineSettings::AutoLoadIconsForPage, false);
    if(m_storagePath.length() <= 0) {
        defaultProfile->setCachePath(qApp->applicationDirPath() + "/webcache/cache");
        defaultProfile->setPersistentStoragePath(qApp->applicationDirPath() + "/webcache/persistent");
    } else {
        defaultProfile->setCachePath(m_storagePath + "/webcache/cache");
        defaultProfile->setPersistentStoragePath(m_storagePath + "/webcache/persistent");
    }
    this->m_view->setPage(this->m_page);
    this->m_cookieStore = this->m_page->profile()->cookieStore();
    this->connect(m_cookieStore, &QWebEngineCookieStore::cookieAdded, &m_cookieJar, &QRestWrapperCookieJar::insertCookie);
    this->connect(m_cookieStore, &QWebEngineCookieStore::cookieRemoved, &m_cookieJar, &QRestWrapperCookieJar::deleteCookie);
    this->connect(this->m_view.get(), &QWebEngineView::urlChanged, this, &QRestWrapperAuthenticator::checkUrlForAuthentication);
    initClientCertificateSelection();
}

void QRestWrapperAuthenticator::initClientCertificateSelection()
{
    this->connect(this->m_page, &QWebEnginePage::selectClientCertificate, [](QWebEngineClientCertificateSelection clientCertificateSelection) {
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
    });
}

/*
 * cookie retrieval/deletion functions
 */

void QRestWrapperAuthenticator::deleteCookiesByName(const QString &cookieName)
{
    return m_cookieJar.deleteCookiesByName(cookieName);
}

void QRestWrapperAuthenticator::deleteCookiesByDomain(const QString &cookieDomain)
{
    return m_cookieJar.deleteCookiesByDomain(cookieDomain);
}

void QRestWrapperAuthenticator::deleteCookiesByPath(const QString &cookiePath)
{
    return m_cookieJar.deleteCookiesByPath(cookiePath);
}

void QRestWrapperAuthenticator::deleteCookiesByNameAndDomain(const QString &cookieName, const QString &cookieDomain)
{
    return m_cookieJar.deleteCookiesByNameAndDomain(cookieName, cookieDomain);
}

void QRestWrapperAuthenticator::deleteCookiesByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath)
{
    return m_cookieJar.deleteCookiesByNameAndDomainAndPath(cookieName, cookieDomain, cookiePath);
}

void QRestWrapperAuthenticator::clearAllCookies()
{
    return m_cookieJar.clearAllCookies();
}

QVector<QNetworkCookie> QRestWrapperAuthenticator::getCookiesByName(const QString &cookieName) const
{
    return m_cookieJar.getCookiesByName(cookieName);
}

QVector<QNetworkCookie> QRestWrapperAuthenticator::getCookiesByDomain(const QString &cookieDomain) const
{
    return m_cookieJar.getCookiesByDomain(cookieDomain);
}

QVector<QNetworkCookie> QRestWrapperAuthenticator::getCookiesByPath(const QString &cookiePath) const
{
    return m_cookieJar.getCookiesByPath(cookiePath);
}

QVector<QNetworkCookie> QRestWrapperAuthenticator::getCookiesByNameAndDomain(const QString &cookieName, const QString &cookieDomain) const
{
    return m_cookieJar.getCookiesByNameAndDomain(cookieName, cookieDomain);
}

QVector<QNetworkCookie> QRestWrapperAuthenticator::getCookiesByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath) const
{
    return m_cookieJar.getCookiesByNameAndDomainAndPath(cookieName, cookieDomain, cookiePath);
}

QVector<QNetworkCookie> QRestWrapperAuthenticator::getAllCookies() const
{
    return m_cookieJar.getAllCookies();
}

QVector<QString> QRestWrapperAuthenticator::getOriginalCookieStringsByName(const QString &cookieName) const
{
    return m_cookieJar.getOriginalCookieStringsByName(cookieName);
}

QVector<QString> QRestWrapperAuthenticator::getOriginalCookieStringsByDomain(const QString &cookieDomain) const
{
    return m_cookieJar.getOriginalCookieStringsByDomain(cookieDomain);
}

QVector<QString> QRestWrapperAuthenticator::getOriginalCookieStringsByPath(const QString &cookiePath) const
{
    return m_cookieJar.getOriginalCookieStringsByPath(cookiePath);
}

QVector<QString> QRestWrapperAuthenticator::getOriginalCookieStringsByNameAndDomain(const QString &cookieName, const QString &cookieDomain) const
{
    return m_cookieJar.getOriginalCookieStringsByNameAndDomain(cookieName, cookieDomain);
}

QVector<QString> QRestWrapperAuthenticator::getOriginalCookieStringsByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath) const
{
    return m_cookieJar.getOriginalCookieStringsByNameAndDomainAndPath(cookieName, cookieDomain, cookiePath);
}

QVector<QString> QRestWrapperAuthenticator::getAllCookieStrings() const
{
    return m_cookieJar.getAllCookieStrings();
}
