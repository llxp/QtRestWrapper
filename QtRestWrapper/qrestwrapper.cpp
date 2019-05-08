#include "qrestwrapper.h"
#include "qrestwrapperurlinterceptor.h"

#include <QWebEngineUrlRequestInfo>
#include <QWebEngineUrlRequestInterceptor>
#include <QWebEngineClientCertificateSelection>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QRegularExpression>
#include <QWebEngineProfile>
#include <QWebEngineCookieStore>
#include <QWebEngineSettings>
#include <QApplication>
#include <QMetaMethod>
#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>

using QtRestWrapper::QRestWrapper;

QRestWrapper::QRestWrapper(QObject *parent) :
    QObject(parent),
    m_networkAccessManager(),
    m_cookieJar(&this->m_networkAccessManager),
    m_urlInterceptor(QSharedPointer<QRestWrapperUrlInterceptor>::create()),
    m_view(nullptr),
    m_page(nullptr)
{
    // set the this pointer to the url interceptor,
    // that it can read the filter lists
    // and invoke authenticationCheck signal
    this->m_urlInterceptor->setRestWrapper(this);
    // connect the finished signal of the QNetworkAccessManager to the waitForCustomRequest slot
    // to be able to capture the result of the rest requests
    connect(&this->m_networkAccessManager, &QNetworkAccessManager::finished, this, &QRestWrapper::waitForCustomRequest);
}

QRestWrapper::~QRestWrapper()
{
    if(m_view != nullptr) {
        m_view->deleteLater();
        m_view = nullptr;
    }
}

void QRestWrapper::setStoragePath(const QString &path)
{
    m_storagePath = path;
}

void QRestWrapper::setAuthenticationTestUrl(const QUrl &url)
{
    m_authenticationTestUrl = url;
}

void QRestWrapper::setApplicationUrl(const QUrl &url)
{
    m_applicationUrl = url;
}

// authentication function
void QRestWrapper::authenticate()
{
    m_isAuthenticated = false;
    initWebEngineView();
    addWebView(this->m_view);
    this->m_view->setUrl(this->m_applicationUrl);
    this->m_view->show();
    this->m_cookieStore = this->m_page->profile()->cookieStore();
}

/*
 * rest sending functions
 */

void QRestWrapper::sendGetRequest(const QUrl &url, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, QSharedPointer<QByteArray>::create(""), "GET", "", header);
}

void QRestWrapper::sendPutRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, QSharedPointer<QByteArray>::create(payload), "PUT", contentType, header);
}

void QRestWrapper::sendPutRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, payload, "PUT", contentType, header);
}

void QRestWrapper::sendDeleteRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, QSharedPointer<QByteArray>::create(payload), "DELETE", contentType, header);
}

void QRestWrapper::sendDeleteRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, payload, "DELETE", contentType, header);
}

void QRestWrapper::sendOptionsRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, QSharedPointer<QByteArray>::create(payload), "OPTIONS", contentType, header);
}

void QRestWrapper::sendOptionsRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, payload, "OPTIONS", contentType, header);
}

void QRestWrapper::sendPostRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, QSharedPointer<QByteArray>::create(payload), "POST", contentType, header);
}

void QRestWrapper::sendPostRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, payload, "POST", contentType, header);
}

void QRestWrapper::sendJsonPostRequest(const QUrl &url, const QJsonDocument &json, const QString &verb, const QMap<QString, QString> &header)
{
    QSharedPointer<QByteArray> jsonData = QSharedPointer<QByteArray>::create(json.toJson());
    return this->sendRequest(url, jsonData, verb, "application/json", header);
}

void QRestWrapper::sendRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &verb, const QString &contentType, const QMap<QString, QString> &header)
{
    QByteArray postDataSize = QByteArray::number(payload->size());

    QNetworkRequest request;

    request.setUrl(url);
    if(contentType.length() > 0) {
        request.setRawHeader("Content-Type", contentType.toUtf8());
    }
    if(payload->size() > 0) {
        request.setRawHeader("Content-Length", postDataSize);
    }

    for(QMap<QString, QString>::const_iterator it = header.begin();it != header.end();it++) {
        request.setRawHeader(it.key().toUtf8(), it->toUtf8());
    }

    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    QString registeredString = randomString();
    m_registeredPayloadData[registeredString] = QPair<QString, QSharedPointer<QByteArray>>(contentType, payload);
    m_registeredCustomHeaders[registeredString] = header;
    QNetworkReply *reply = this->m_networkAccessManager.sendCustomRequest(request, QByteArray(verb.toUtf8()), *payload);
    reply->setProperty("registeredString", registeredString);
}

/*
 * cookie retrieval/deletion functions
 */

void QRestWrapper::deleteCookiesByName(const QString &cookieName)
{
    return m_cookieJar.deleteCookiesByName(cookieName);
}

void QRestWrapper::deleteCookiesByDomain(const QString &cookieDomain)
{
    return m_cookieJar.deleteCookiesByDomain(cookieDomain);
}

void QRestWrapper::deleteCookiesByPath(const QString &cookiePath)
{
    return m_cookieJar.deleteCookiesByPath(cookiePath);
}

void QRestWrapper::deleteCookiesByNameAndDomain(const QString &cookieName, const QString &cookieDomain)
{
    return m_cookieJar.deleteCookiesByNameAndDomain(cookieName, cookieDomain);
}

void QRestWrapper::deleteCookiesByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath)
{
    return m_cookieJar.deleteCookiesByNameAndDomainAndPath(cookieName, cookieDomain, cookiePath);
}

void QRestWrapper::clearAllCookies()
{
    return m_cookieJar.clearAllCookies();
}

QVector<QNetworkCookie> QRestWrapper::getCookiesByName(const QString &cookieName) const
{
    return m_cookieJar.getCookiesByName(cookieName);
}

QVector<QNetworkCookie> QRestWrapper::getCookiesByDomain(const QString &cookieDomain) const
{
    return m_cookieJar.getCookiesByDomain(cookieDomain);
}

QVector<QNetworkCookie> QRestWrapper::getCookiesByPath(const QString &cookiePath) const
{
    return m_cookieJar.getCookiesByPath(cookiePath);
}

QVector<QNetworkCookie> QRestWrapper::getCookiesByNameAndDomain(const QString &cookieName, const QString &cookieDomain) const
{
    return m_cookieJar.getCookiesByNameAndDomain(cookieName, cookieDomain);
}

QVector<QNetworkCookie> QRestWrapper::getCookiesByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath) const
{
    return m_cookieJar.getCookiesByNameAndDomainAndPath(cookieName, cookieDomain, cookiePath);
}

QVector<QNetworkCookie> QRestWrapper::getAllCookies() const
{
    return m_cookieJar.getAllCookies();
}

QVector<QString> QRestWrapper::getOriginalCookieStringsByName(const QString &cookieName) const
{
    return m_cookieJar.getOriginalCookieStringsByName(cookieName);
}

QVector<QString> QRestWrapper::getOriginalCookieStringsByDomain(const QString &cookieDomain) const
{
    return m_cookieJar.getOriginalCookieStringsByDomain(cookieDomain);
}

QVector<QString> QRestWrapper::getOriginalCookieStringsByPath(const QString &cookiePath) const
{
    return m_cookieJar.getOriginalCookieStringsByPath(cookiePath);
}

QVector<QString> QRestWrapper::getOriginalCookieStringsByNameAndDomain(const QString &cookieName, const QString &cookieDomain) const
{
    return m_cookieJar.getOriginalCookieStringsByNameAndDomain(cookieName, cookieDomain);
}

QVector<QString> QRestWrapper::getOriginalCookieStringsByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath) const
{
    return m_cookieJar.getOriginalCookieStringsByNameAndDomainAndPath(cookieName, cookieDomain, cookiePath);
}

QVector<QString> QRestWrapper::getAllCookieStrings() const
{
    return m_cookieJar.getAllCookieStrings();
}

/*
 * random string utility functions
 */

QVector<QString> QRestWrapper::m_generatedRandomStrings;

int constexpr strLength(const char* str)
{
    return *str ? 1 + strLength(str + 1) : 0;
}

QString QRestWrapper::randomString(int length)
{
    static constexpr char possibleCharacters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    static constexpr int possibleCharactersLength = strLength(possibleCharacters);

    QString randomString;
    while(m_generatedRandomStrings.contains(randomString)) {
        randomString = "";
        for (int i=0; i<length; ++i) {
            int index = qrand() % possibleCharactersLength;
            QChar nextChar = possibleCharacters[index];
            randomString.append(nextChar);
        }
    }
    m_generatedRandomStrings.append(randomString);
    return randomString;
}

void QRestWrapper::checkUrlForAuthentication(const QUrl &url)
{
    // check if the 'QRestWrapper::authenticationCheck' signal is connected
    if(isSignalConnected(QMetaMethod::fromSignal(&QRestWrapper::authenticationCheck))) {
        // the signal is connected
        if(!this->m_isAuthenticated && authenticationCheck(url)) {
            this->m_networkAccessManager.setCookieJar(&m_cookieJar);
            this->m_isAuthenticated = true;
            disconnect(this->m_view, &QWebEngineView::urlChanged, this, &QRestWrapper::checkUrlForAuthentication);
            removeWebView(this->m_view);
            this->m_view->deleteLater();
            this->m_view = nullptr;
            this->m_page->deleteLater();
            this->m_page = nullptr;
            authenticated(url);
        }
    } else {
        // the signal is not connected
        QRegularExpression re(this->m_authenticationTestUrl.toString(), QRegularExpression::DotMatchesEverythingOption);
        QRegularExpressionMatch match = re.match(QUrl::fromPercentEncoding(url.toString().toUtf8()));
        if (!this->m_isAuthenticated && match.hasMatch()) {
            this->m_networkAccessManager.setCookieJar(&m_cookieJar);
            this->m_isAuthenticated = true;
            disconnect(this->m_view, &QWebEngineView::urlChanged, this, &QRestWrapper::checkUrlForAuthentication);
            removeWebView(this->m_view);
            this->m_view->deleteLater();
            this->m_view = nullptr;
            this->m_page->deleteLater();
            this->m_page = nullptr;
            authenticated(url);
        }
    }
}

void QRestWrapper::waitForCustomRequest(QNetworkReply *reply)
{
    QString registeredString = reply->property("registeredString").toString();
    int statusCode = reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute).toInt();
    switch(statusCode) {
        case 200: {
            // request was successful
            QByteArray content = reply->read(1024);
            content += reply->readAll();
            contentReady(reply->url(), content);
            cleanupRequest(registeredString);
            break;
        }
        case 301:
        case 302:
        case 303:
        case 305:
        case 307:
        case 308: {
            // redirect requested
            QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            QUrl redirectUrl;
            if(!possibleRedirectUrl.isNull() && possibleRedirectUrl.isValid() && possibleRedirectUrl != reply->request().url()) {
                QVariant customVerb = reply->request().attribute(QNetworkRequest::CustomVerbAttribute);
                sendRequest(possibleRedirectUrl.toUrl(),
                            m_registeredPayloadData[registeredString].second,
                            customVerb.toString(),
                            m_registeredPayloadData[registeredString].first,
                            m_registeredCustomHeaders[registeredString]);
            }
            break;
        }
        case 401:
        case 403: {
            // authentication failure
            QByteArray content = reply->read(1024);
            content += reply->readAll();
            authenticationFailure(reply->url(), content);
            cleanupRequest(registeredString);
            break;
        }
        case 404: {
            // the url was not found
            QByteArray content = reply->read(1024);
            content += reply->readAll();
            notFound(reply->url(), content);
            cleanupRequest(registeredString);
            break;
        }
        default: {
            // some other not defined http header was returned
            qDebug() << "statusCode: " << statusCode;
            QByteArray content = reply->read(1024);
            content += reply->readAll();
            undefinedStatusCode(reply->url(), content);
            cleanupRequest(registeredString);
            break;
        }
    }
    reply->deleteLater();
}

void QRestWrapper::initWebEngineView()
{
    this->m_view = new QWebEngineView();
    this->m_view->setEnabled(true);
    QWebEngineProfile* defaultProfile = QWebEngineProfile::defaultProfile();
    connect(this->m_urlInterceptor.get(), &QRestWrapperUrlInterceptor::checkAuthenticationPage, this, &QRestWrapper::checkUrlForAuthentication);
    defaultProfile->setRequestInterceptor(this->m_urlInterceptor.get());
    defaultProfile->setPersistentCookiesPolicy(QWebEngineProfile::AllowPersistentCookies);
    this->m_page = new QWebEnginePage(defaultProfile, this->m_view);
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
    connect(m_cookieStore, &QWebEngineCookieStore::cookieAdded, &m_cookieJar, &QRestWrapperCookieJar::insertCookie);
    connect(m_cookieStore, &QWebEngineCookieStore::cookieRemoved, &m_cookieJar, &QRestWrapperCookieJar::deleteCookie);
    connect(this->m_view, &QWebEngineView::urlChanged, this, &QRestWrapper::checkUrlForAuthentication);
    initClientCertificateSelection();
}

void QRestWrapper::initClientCertificateSelection()
{
    QObject::connect(this->m_page, &QWebEnginePage::selectClientCertificate, [this](QWebEngineClientCertificateSelection clientCertificateSelection) {
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
        QObject::connect(certList, &QTableWidget::clicked, [&selectedIndex, okButton](const QModelIndex &index) {
            selectedIndex = index;
            okButton->setDisabled(false);
        });
        QObject::connect(certList, &QTableWidget::doubleClicked, [certSelectionDialog, &clientCertificateSelection](const QModelIndex &index) {
            clientCertificateSelection.select(clientCertificateSelection.certificates().at(index.row()));
            certSelectionDialog->close();
            certSelectionDialog->deleteLater();
        });
        certSelectionDialog->setLayout(new QVBoxLayout(certSelectionDialog));
        certSelectionDialog->layout()->addWidget(certList);
        QObject::connect(okButton, &QPushButton::clicked, [certSelectionDialog, &clientCertificateSelection, &selectedIndex]() {
            clientCertificateSelection.select(clientCertificateSelection.certificates().at(selectedIndex.row()));
            certSelectionDialog->close();
            certSelectionDialog->deleteLater();
        });
        okButton->setDisabled(true);
        okButton->setText("Ok");
        okButton->setFixedWidth(100);
        okButton->setFixedHeight(50);
        QObject::connect(certSelectionDialog, &QDialog::accepted, [certSelectionDialog, &clientCertificateSelection, &selectedIndex]() {
            clientCertificateSelection.select(clientCertificateSelection.certificates().at(selectedIndex.row()));
            certSelectionDialog->deleteLater();
        });
        QObject::connect(certSelectionDialog, &QDialog::rejected, [certSelectionDialog]() {
            certSelectionDialog->deleteLater();
        });
        certSelectionDialog->layout()->addWidget(okButton);
        certSelectionDialog->exec();
    });
}

void QRestWrapper::cleanupRequest(const QString &registeredString)
{
    typedef QMap<QString, QPair<QString, QSharedPointer<QByteArray>>>::iterator RegisteredPayloadConstIterator;
    typedef QMap<QString, QMap<QString, QString>>::iterator RegisteredCustomHeadersConstIterators;
    for(RegisteredPayloadConstIterator it = m_registeredPayloadData.begin(); it != m_registeredPayloadData.end();) {
        if(it.key() == registeredString) {
            it = m_registeredPayloadData.erase(it);
        } else {
            ++it;
        }
    }
    for(RegisteredCustomHeadersConstIterators it = m_registeredCustomHeaders.begin(); it != m_registeredCustomHeaders.end();) {
        if(it.key() == registeredString) {
            it = m_registeredCustomHeaders.erase(it);
        } else {
            ++it;
        }
    }
}
