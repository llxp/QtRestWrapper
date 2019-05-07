#ifndef QTRESTWRAPPER_H
#define QTRESTWRAPPER_H

#include "qrestwrappercookiejar.h"
#include "qtrestwrapper_global.h"

#include <QMap>
#include <QNetworkCookie>
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QSharedPointer>
#include <QJsonDocument>
#include <QUrl>
#include <QNetworkReply>
#include <QVector>

class QWebEngineView;
class QWebEngineClientCertificateSelection;
class QWebEngineCookieStore;
class QWebEnginePage;

namespace QtRestWrapper {

class QRestWrapperUrlInterceptor;
class QRestWrapperTest;

class QTRESTWRAPPERSHARED_EXPORT QRestWrapper : public QObject
{
    Q_OBJECT
public:
    explicit QRestWrapper(QObject *parent = nullptr);
    ~QRestWrapper();

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
    void sendJsonPostRequest(const QUrl &url, const QJsonDocument &json, const QString &verb, const QMap<QString, QString> &header = QMap<QString, QString>());
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
    QMap<QPair<QString, QString>, QNetworkCookie> getAllCookies() const;

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

signals:
    // will be called during authentication phase
    // when a webengineview has been created, it can be attached to a widget using this signal
    // if not connected a new window with the webengineview will be shown
    void addWebView(QWebEngineView *view);
    // will be called when the authentication phase is over
    // when the webview should be detached from a widget, this signal has to be connected
    // if not connected, the shown webengineview will closed
    void removeWebView(QWebEngineView *view);
    // will be called, when the authenticated reached the point of a valid authenticated
    void authenticated(const QUrl &url);
    // will be called (if connected) if a custom authenticated check is needed
    // return true, if the authentication check was successful
    // otherwise, the authentication phase will just continue
    bool authenticationCheck(const QUrl &url);

signals:
    // these signals will be called, once there has been a reply from a rest request
    // check for the url, to see, if the signal matches the right request
    void contentReady(const QUrl &url, const QByteArray &content); // status 200
    void authenticationFailure(const QUrl &url, const QByteArray &content); // status 40*
    void notFound(const QUrl &url, const QByteArray &content); // status 404
    void undefinedStatusCode(const QUrl &url, const QByteArray &content); // every other status code

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
    QWebEnginePage *m_page;
    QWebEngineView *m_view;
    QWebEngineCookieStore *m_cookieStore { nullptr };
    QRestWrapperCookieJar m_cookieJar;
    QUrl m_authenticationTestUrl { "" };
    QUrl m_applicationUrl {""};
    QSharedPointer<QRestWrapperUrlInterceptor> m_urlInterceptor;
    bool m_isAuthenticated { false };
    QString m_storagePath;

protected:
    QVector<QString> m_allowedUrlPatterns;
    QVector<QString> m_forbiddenUrlPatterns;

private:
    static QVector<QString> m_generatedRandomStrings;

private:
    friend class QRestWrapperTest;
};

}  // namespace QtRestWrapper

#endif // QTRESTWRAPPER_H
