#include "qrestwrapper.h"
#include "private/qrestwrapper_p.h"

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
    QObject(parent), d_ptr(new QRestWrapperPrivate(this))
{
}

QRestWrapper::~QRestWrapper()
{
}

void QRestWrapper::setStoragePath(const QString &path)
{
    Q_D(QRestWrapper);
    d->setStoragePath(path);
}

void QRestWrapper::setAuthenticationTestUrl(const QUrl &url)
{
    Q_D(QRestWrapper);
    d->setAuthenticationTestUrl(url);
}

void QRestWrapper::setApplicationUrl(const QUrl &url)
{
    Q_D(QRestWrapper);
    d->setApplicationUrl(url);
}

void QRestWrapper::authenticate()
{
    Q_D(QRestWrapper);
    d->authenticate();
}

void QRestWrapper::runJavaScript(const QString &scriptSource)
{
    Q_D(QRestWrapper);
    d->runJavaScript(scriptSource);
}

QVariant QRestWrapper::runJavaScriptSynchronous(const QString &scriptSource)
{
    Q_D(QRestWrapper);
    return d->runJavaScriptSynchronous(scriptSource);
}

void QRestWrapper::sendGetRequest(const QUrl &url, const QMap<QString, QString> &header)
{
    Q_D(QRestWrapper);
    d->sendGetRequest(url, header);
}

void QRestWrapper::sendPutRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    Q_D(QRestWrapper);
    d->sendPutRequest(url, payload, contentType, header);
}

void QRestWrapper::sendPutRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    Q_D(QRestWrapper);
    d->sendPutRequest(url, payload, contentType, header);
}

void QRestWrapper::sendDeleteRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    Q_D(QRestWrapper);
    d->sendDeleteRequest(url, payload, contentType, header);
}

void QRestWrapper::sendDeleteRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    Q_D(QRestWrapper);
    d->sendDeleteRequest(url, payload, contentType, header);
}

void QRestWrapper::sendOptionsRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    Q_D(QRestWrapper);
    d->sendOptionsRequest(url, payload, contentType, header);
}

void QRestWrapper::sendOptionsRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    Q_D(QRestWrapper);
    d->sendOptionsRequest(url, payload, contentType, header);
}

void QRestWrapper::sendPostRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    Q_D(QRestWrapper);
    d->sendPostRequest(url, payload, contentType, header);
}

void QRestWrapper::sendPostRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    Q_D(QRestWrapper);
    d->sendPostRequest(url, payload, contentType, header);
}

void QRestWrapper::sendJsonRequest(const QUrl &url, const QJsonDocument &json, const QString &verb, const QMap<QString, QString> &header)
{
    Q_D(QRestWrapper);
    d->sendJsonRequest(url, json, verb, header);
}

void QRestWrapper::sendRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &verb, const QString &contentType, const QMap<QString, QString> &header)
{
    Q_D(QRestWrapper);
    d->sendRequest(url, payload, verb, contentType, header);
}

void QRestWrapper::deleteCookiesByName(const QString &cookieName)
{
    Q_D(QRestWrapper);
    d->deleteCookiesByName(cookieName);
}

void QRestWrapper::deleteCookiesByDomain(const QString &cookieDomain)
{
    Q_D(QRestWrapper);
    d->deleteCookiesByDomain(cookieDomain);
}

void QRestWrapper::deleteCookiesByPath(const QString &cookiePath)
{
    Q_D(QRestWrapper);
    d->deleteCookiesByPath(cookiePath);
}

void QRestWrapper::deleteCookiesByNameAndDomain(const QString &cookieName, const QString &cookieDomain)
{
    Q_D(QRestWrapper);
    d->deleteCookiesByNameAndDomain(cookieName, cookieDomain);
}

void QRestWrapper::deleteCookiesByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath)
{
    Q_D(QRestWrapper);
    d->deleteCookiesByNameAndDomainAndPath(cookieName, cookieDomain, cookiePath);
}

void QRestWrapper::clearAllCookies()
{
    Q_D(QRestWrapper);
    d->clearAllCookies();
}

QVector<QNetworkCookie> QRestWrapper::getCookiesByName(const QString &cookieName) const
{
    Q_D(const QRestWrapper);
    return d->getCookiesByName(cookieName);
}

QVector<QNetworkCookie> QRestWrapper::getCookiesByDomain(const QString &cookieDomain) const
{
    Q_D(const QRestWrapper);
    return d->getCookiesByDomain(cookieDomain);
}

QVector<QNetworkCookie> QRestWrapper::getCookiesByPath(const QString &cookiePath) const
{
    Q_D(const QRestWrapper);
    return d->getCookiesByPath(cookiePath);
}

QVector<QNetworkCookie> QRestWrapper::getCookiesByNameAndDomain(const QString &cookieName, const QString &cookieDomain) const
{
    Q_D(const QRestWrapper);
    return d->getCookiesByNameAndDomain(cookieName, cookieDomain);
}

QVector<QNetworkCookie> QRestWrapper::getCookiesByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath) const
{
    Q_D(const QRestWrapper);
    return d->getCookiesByNameAndDomainAndPath(cookieName, cookieDomain, cookiePath);
}

QVector<QNetworkCookie> QRestWrapper::getAllCookies() const
{
    Q_D(const QRestWrapper);
    return d->getAllCookies();
}

QVector<QString> QRestWrapper::getOriginalCookieStringsByName(const QString &cookieName) const
{
    Q_D(const QRestWrapper);
    return d->getOriginalCookieStringsByName(cookieName);
}

QVector<QString> QRestWrapper::getOriginalCookieStringsByDomain(const QString &cookieDomain) const
{
    Q_D(const QRestWrapper);
    return d->getOriginalCookieStringsByDomain(cookieDomain);
}

QVector<QString> QRestWrapper::getOriginalCookieStringsByPath(const QString &cookiePath) const
{
    Q_D(const QRestWrapper);
    return d->getOriginalCookieStringsByPath(cookiePath);
}

QVector<QString> QRestWrapper::getOriginalCookieStringsByNameAndDomain(const QString &cookieName, const QString &cookieDomain) const
{
    Q_D(const QRestWrapper);
    return d->getOriginalCookieStringsByNameAndDomain(cookieName, cookieDomain);
}

QVector<QString> QRestWrapper::getOriginalCookieStringsByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath) const
{
    Q_D(const QRestWrapper);
    return d->getOriginalCookieStringsByNameAndDomainAndPath(cookieName, cookieDomain, cookiePath);
}

QVector<QString> QRestWrapper::getAllCookieStrings() const
{
    Q_D(const QRestWrapper);
    return d->getAllCookieStrings();
}

QString QRestWrapper::randomString(int length)
{
    return QRestWrapperPrivate::randomString(length);
}

QMap<QString, QString> QRestWrapper::cookieSplit(const QString &cookieString)
{
    return QRestWrapperPrivate::cookieSplit(cookieString);
}

void QtRestWrapper::QRestWrapper::connectNotify(const QMetaMethod &signal)
{
    Q_D(QRestWrapper);
    d->connectNotify(signal);
}

void QRestWrapper::disconnectNotify(const QMetaMethod &signal)
{
    Q_D(QRestWrapper);
    d->disconnectNotify(signal);
}
