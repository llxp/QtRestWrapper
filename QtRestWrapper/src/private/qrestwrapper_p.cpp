#include "../../include/qrestwrapper.h"
#include "../../include/private/qrestwrapper_p.h"
#include "../../include/private/qrestwrapperutils.h"

namespace QtRestWrapper {

QRestWrapperPrivate::QRestWrapperPrivate(QRestWrapper *q)
    : m_authenticator(new QRestWrapperAuthenticator(q)),
      m_networkHandler(new QRestWrapperNetworkHandler(q)),
      q_ptr(q)
{
    q->connect(m_authenticator, &QRestWrapperAuthenticator::addWebView, q, &QRestWrapper::addWebView);
    q->connect(m_authenticator, &QRestWrapperAuthenticator::closed, q, &QRestWrapper::closed);
    q->connect(m_authenticator, &QRestWrapperAuthenticator::authenticated, q, &QRestWrapper::authenticated);
    q->connect(m_authenticator, &QRestWrapperAuthenticator::authenticatedContent, q, &QRestWrapper::authenticatedContent);
    q->connect(m_authenticator, &QRestWrapperAuthenticator::authenticationCheck, q, &QRestWrapper::authenticationCheck);
    q->connect(m_authenticator, &QRestWrapperAuthenticator::authenticationCheckContent, q, &QRestWrapper::authenticationCheckContent);
    q->connect(m_authenticator, &QRestWrapperAuthenticator::certificateError, q, &QRestWrapper::certificateError);

    q->connect(m_authenticator, &QRestWrapperAuthenticator::scriptEnded, q, &QRestWrapper::scriptEnded);

    q->connect(m_networkHandler, &QRestWrapperNetworkHandler::contentReady, q, &QRestWrapper::contentReady);
    q->connect(m_networkHandler, &QRestWrapperNetworkHandler::authenticationFailure, q, &QRestWrapper::authenticationFailure);
    q->connect(m_networkHandler, &QRestWrapperNetworkHandler::notFound, q, &QRestWrapper::notFound);
    q->connect(m_networkHandler, &QRestWrapperNetworkHandler::undefinedStatusCode, q, &QRestWrapper::undefinedStatusCode);
}

QRestWrapperPrivate::~QRestWrapperPrivate()
{
    qDebug() << __FUNCTION__;
    m_authenticator->disconnect();
    m_networkHandler->disconnect();
    m_authenticator->deleteLater();
    m_networkHandler->deleteLater();
}

void QRestWrapperPrivate::setStoragePath(const QString &path)
{
    m_authenticator->setStoragePath(path);
}

void QRestWrapperPrivate::setAuthenticationTestUrl(const QUrl &url)
{
    m_authenticator->setAuthenticationTestUrl(url);
}

void QRestWrapperPrivate::setApplicationUrl(const QUrl &url)
{
    m_authenticator->setApplicationUrl(url);
}

// authentication function
void QRestWrapperPrivate::authenticate()
{
    this->m_authenticator->authenticate();
}

/*
 * rest sending functions
 */

void QRestWrapperPrivate::sendGetRequest(const QUrl &url, const QMap<QString, QString> &header)
{
    m_networkHandler->sendRequest(url, QSharedPointer<QByteArray>::create(""), "GET", "", header);
}

void QRestWrapperPrivate::sendPutRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    m_networkHandler->sendRequest(url, QSharedPointer<QByteArray>::create(payload), "PUT", contentType, header);
}

void QRestWrapperPrivate::sendPutRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    m_networkHandler->sendRequest(url, payload, "PUT", contentType, header);
}

void QRestWrapperPrivate::sendDeleteRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    m_networkHandler->sendRequest(url, QSharedPointer<QByteArray>::create(payload), "DELETE", contentType, header);
}

void QRestWrapperPrivate::sendDeleteRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    m_networkHandler->sendRequest(url, payload, "DELETE", contentType, header);
}

void QRestWrapperPrivate::sendOptionsRequest(const QUrl &url, const QMap<QString, QString> &header)
{
    m_networkHandler->sendRequest(url, QSharedPointer<QByteArray>::create(""), "OPTIONS", "", header);
}

void QRestWrapperPrivate::sendPostRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    m_networkHandler->sendRequest(url, QSharedPointer<QByteArray>::create(payload), "POST", contentType, header);
}

void QRestWrapperPrivate::sendPostRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    m_networkHandler->sendRequest(url, payload, "POST", contentType, header);
}

void QRestWrapperPrivate::sendJsonRequest(const QUrl &url, const QJsonDocument &json, const QString &verb, const QMap<QString, QString> &header)
{
    QSharedPointer<QByteArray> jsonData = QSharedPointer<QByteArray>::create(json.toJson());
    m_networkHandler->sendRequest(url, jsonData, verb, "application/json", header);
}

void QRestWrapperPrivate::sendRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &verb, const QString &contentType, const QMap<QString, QString> &header)
{
    m_networkHandler->sendRequest(url, payload, verb, contentType, header);
}

/*
 * cookie retrieval/deletion functions
 */

void QRestWrapperPrivate::deleteCookiesByName(const QString &cookieName)
{
    m_authenticator->deleteCookiesByName(cookieName);
}

void QRestWrapperPrivate::deleteCookiesByDomain(const QString &cookieDomain)
{
    m_authenticator->deleteCookiesByDomain(cookieDomain);
}

void QRestWrapperPrivate::deleteCookiesByPath(const QString &cookiePath)
{
    m_authenticator->deleteCookiesByPath(cookiePath);
}

void QRestWrapperPrivate::deleteCookiesByNameAndDomain(const QString &cookieName, const QString &cookieDomain)
{
    m_authenticator->deleteCookiesByNameAndDomain(cookieName, cookieDomain);
}

void QRestWrapperPrivate::deleteCookiesByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath)
{
    m_authenticator->deleteCookiesByNameAndDomainAndPath(cookieName, cookieDomain, cookiePath);
}

void QRestWrapperPrivate::clearAllCookies()
{
    m_authenticator->clearAllCookies();
}

QVector<QNetworkCookie> QRestWrapperPrivate::getCookiesByName(const QString &cookieName) const
{
    return m_authenticator->getCookiesByName(cookieName);
}

QVector<QNetworkCookie> QRestWrapperPrivate::getCookiesByDomain(const QString &cookieDomain) const
{
    return m_authenticator->getCookiesByDomain(cookieDomain);
}

QVector<QNetworkCookie> QRestWrapperPrivate::getCookiesByPath(const QString &cookiePath) const
{
    return m_authenticator->getCookiesByPath(cookiePath);
}

QVector<QNetworkCookie> QRestWrapperPrivate::getCookiesByNameAndDomain(const QString &cookieName, const QString &cookieDomain) const
{
    return m_authenticator->getCookiesByNameAndDomain(cookieName, cookieDomain);
}

QVector<QNetworkCookie> QRestWrapperPrivate::getCookiesByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath) const
{
    return m_authenticator->getCookiesByNameAndDomainAndPath(cookieName, cookieDomain, cookiePath);
}

QVector<QNetworkCookie> QRestWrapperPrivate::getAllCookies() const
{
    return m_authenticator->getAllCookies();
}

QVector<QString> QRestWrapperPrivate::getOriginalCookieStringsByName(const QString &cookieName) const
{
    return m_authenticator->getOriginalCookieStringsByName(cookieName);
}

QVector<QString> QRestWrapperPrivate::getOriginalCookieStringsByDomain(const QString &cookieDomain) const
{
    return m_authenticator->getOriginalCookieStringsByDomain(cookieDomain);
}

QVector<QString> QRestWrapperPrivate::getOriginalCookieStringsByPath(const QString &cookiePath) const
{
    return m_authenticator->getOriginalCookieStringsByPath(cookiePath);
}

QVector<QString> QRestWrapperPrivate::getOriginalCookieStringsByNameAndDomain(const QString &cookieName, const QString &cookieDomain) const
{
    return m_authenticator->getOriginalCookieStringsByNameAndDomain(cookieName, cookieDomain);
}

QVector<QString> QRestWrapperPrivate::getOriginalCookieStringsByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath) const
{
    return m_authenticator->getOriginalCookieStringsByNameAndDomainAndPath(cookieName, cookieDomain, cookiePath);
}

QVector<QString> QRestWrapperPrivate::getAllCookieStrings() const
{
    return m_authenticator->getAllCookieStrings();
}

void QRestWrapperPrivate::runJavaScript(const QString &scriptSource)
{
    m_authenticator->runJavaScript(scriptSource);
}

QVariant QRestWrapperPrivate::runJavaScriptSynchronous(const QString &scriptSource)
{
    return m_authenticator->runJavaScriptSynchronous(scriptSource);
}

QString QRestWrapperPrivate::randomString(int length)
{
    return QRestWrapperUtils::randomString(length);
}

QMap<QString, QString> QRestWrapperPrivate::cookieSplit(const QString &cookieString)
{
    return QRestWrapperUtils::cookieSplit(cookieString);
}

void QRestWrapperPrivate::connectNotify(const QMetaMethod &signal)
{
    Q_Q(QRestWrapper);
    if (signal == QMetaMethod::fromSignal(&QRestWrapper::authenticationCheck)) {
        if(q->isSignalConnected(signal)) {
            m_authenticator->setAlternateAuthenticationCheck(true);
        }
    } else if (signal == QMetaMethod::fromSignal(&QRestWrapper::authenticationCheckContent)) {
        if(q->isSignalConnected(signal)) {
            m_authenticator->setAlternateAuthenticationCheck(true);
            m_authenticator->setFetchContentOnAuthenticationCheck(true);
        }
    } else if (signal == QMetaMethod::fromSignal(&QRestWrapper::authenticatedContent)) {
        if(q->isSignalConnected(signal)) {
            m_authenticator->setFetchContentOnAuthenticated(true);
        }
    }
}

void QRestWrapperPrivate::disconnectNotify(const QMetaMethod &signal)
{
    Q_Q(QRestWrapper);
    if (signal == QMetaMethod::fromSignal(&QRestWrapper::authenticationCheck)) {
        if(!q->isSignalConnected(signal) &&
           !q->isSignalConnected(QMetaMethod::fromSignal(&QRestWrapper::authenticationCheckContent))) {
            m_authenticator->setAlternateAuthenticationCheck(false);
        }
    } else if (signal == QMetaMethod::fromSignal(&QRestWrapper::authenticationCheckContent)) {
        if(!q->isSignalConnected(signal) &&
           !q->isSignalConnected(QMetaMethod::fromSignal(&QRestWrapper::authenticationCheck))) {
            m_authenticator->setAlternateAuthenticationCheck(false);
        }
    } else if (signal == QMetaMethod::fromSignal(&QRestWrapper::authenticatedContent)) {
        if(!q->isSignalConnected(signal)) {
            m_authenticator->setFetchContentOnAuthenticated(false);
        }
    }
}

} // namespace QtRestWrapper
