#ifndef QRESTWRAPPER_P_H
#define QRESTWRAPPER_P_H

#include "qrestwrappercookiejar.h"
#include "qrestwrapperurlinterceptor.h"
#include "qrestwrapper.h"

#include <QMap>
#include <QNetworkCookie>
#include <QNetworkReply>
#include <QSharedPointer>
#include <QString>
#include <QUrl>
#include <QWebEngineCookieStore>
#include <QWebEngineView>



namespace QtRestWrapper {

class QRestWrapperPrivate
{
    Q_DECLARE_PUBLIC(QRestWrapper)
public:
    QRestWrapperPrivate(QRestWrapper *q);
    ~QRestWrapperPrivate();

public:
    void setStoragePath(const QString &path);  // default will be the applications path, path needs to end without '/'
    void setAuthenticationTestUrl(const QUrl &url);
    void setApplicationUrl(const QUrl &url);
    void authenticate();

public:
    void sendGetRequest(const QUrl &url, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendPutRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendPutRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendDeleteRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendDeleteRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendOptionsRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendOptionsRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendPostRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendPostRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendJsonRequest(const QUrl &url, const QJsonDocument &json, const QString &verb, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &verb, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());

public:
    void deleteCookiesByName(const QString &cookieName);
    void deleteCookiesByDomain(const QString &cookieDomain);
    void deleteCookiesByPath(const QString &cookiePath);
    void deleteCookiesByNameAndDomain(const QString &cookieName, const QString &cookieDomain);
    void deleteCookiesByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath);
    void clearAllCookies();

public:
    QVector<QNetworkCookie> getCookiesByName(const QString &cookieName) const;
    QVector<QNetworkCookie> getCookiesByDomain(const QString &cookieDomain) const;
    QVector<QNetworkCookie> getCookiesByPath(const QString &cookiePath) const;
    QVector<QNetworkCookie> getCookiesByNameAndDomain(const QString &cookieName, const QString &cookieDomain) const;
    QVector<QNetworkCookie> getCookiesByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath) const;
    QVector<QNetworkCookie> getAllCookies() const;

public:
    QVector<QString> getOriginalCookieStringsByName(const QString &cookieName) const;
    QVector<QString> getOriginalCookieStringsByDomain(const QString &cookieDomain) const;
    QVector<QString> getOriginalCookieStringsByPath(const QString &cookiePath) const;
    QVector<QString> getOriginalCookieStringsByNameAndDomain(const QString &cookieName, const QString &cookieDomain) const;
    QVector<QString> getOriginalCookieStringsByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath) const;
    QVector<QString> getAllCookieStrings() const;

public:
    // generates a unique random string, unique only, when only this function will be used to generate the random string
    static QString randomString(int length = 12);
    // utility function to split a cookie string into individual raw cookies
    static std::map<QString, QString> cookieSplit(const QString &cookieString);

private slots:
    void checkUrlForAuthentication(const QUrl &url);
    void waitForCustomRequest(QNetworkReply *reply);

private:
    void initWebEngineView();
    void initClientCertificateSelection();
    void cleanupRequest(const QString &registeredString);

private:
    QNetworkAccessManager m_networkAccessManager;
    QMap<QString, QPair<QString, QSharedPointer<QByteArray>>> m_registeredPayloadData;
    QMap<QString, QMap<QString, QString>> m_registeredCustomHeaders;

private:
    QRestWrapperCookieJar m_cookieJar;
    QSharedPointer<QRestWrapperUrlInterceptor> m_urlInterceptor;
    QWebEngineView *m_view;
    QWebEnginePage *m_page;
    QWebEngineCookieStore *m_cookieStore { nullptr };
    QUrl m_authenticationTestUrl { "" };
    QUrl m_applicationUrl {""};
    bool m_isAuthenticated { false };
    QString m_storagePath;

protected:
    QVector<QString> m_allowedUrlPatterns;
    QVector<QString> m_forbiddenUrlPatterns;

private:
    static QVector<QString> m_generatedRandomStrings;

private:
    QRestWrapper *q_ptr;
};

} // namespace QtRestWrapper

#endif // QRESTWRAPPER_P_H
