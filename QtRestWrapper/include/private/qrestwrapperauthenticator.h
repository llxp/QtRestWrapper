#ifndef QRESTWRAPPERURLCHECKER_H
#define QRESTWRAPPERURLCHECKER_H

#include "../qrestwrappercertificateerror.h"
#include "qrestwrappercookiejar.h"
#include "qrestwrapperjavascriptinjector.h"
#include "qrestwrapperpage.h"
#include "qrestwrapperurlinterceptor.h"
#include "qrestwrapperview.h"

#include <QNetworkReply>
#include <QObject>
#include <QWebEngineCookieStore>
#include <QWebEngineView>

namespace QtRestWrapper {
class QTRESTWRAPPERSHARED_EXPORT QRestWrapperAuthenticator : public QObject
{
    Q_OBJECT
public:
    explicit QRestWrapperAuthenticator(QObject *parent = nullptr);
    ~QRestWrapperAuthenticator();

public slots:
    void authenticate();

public:
    void setStoragePath(const QString &path);  // default will be the applications path, path needs to end without '/'
    void setAuthenticationTestUrl(const QUrl &url);
    void setApplicationUrl(const QUrl &url);
    void setAlternateAuthenticationCheck(bool alternateAuthenticationCheck);
    void setFetchContentOnAuthenticated(bool fetchContentOnAuthenticated);
    void setFetchContentOnAuthenticationCheck(bool fetchContentOnAuthenticationCheck);

public slots:
    void deleteCookiesByName(const QString &cookieName);
    void deleteCookiesByDomain(const QString &cookieDomain);
    void deleteCookiesByPath(const QString &cookiePath);
    void deleteCookiesByNameAndDomain(const QString &cookieName, const QString &cookieDomain);
    void deleteCookiesByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath);
    void clearAllCookies();

public slots:
    QVector<QNetworkCookie> getCookiesByName(const QString &cookieName) const;
    QVector<QNetworkCookie> getCookiesByDomain(const QString &cookieDomain) const;
    QVector<QNetworkCookie> getCookiesByPath(const QString &cookiePath) const;
    QVector<QNetworkCookie> getCookiesByNameAndDomain(const QString &cookieName, const QString &cookieDomain) const;
    QVector<QNetworkCookie> getCookiesByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath) const;
    QVector<QNetworkCookie> getAllCookies() const;

public slots:
    QVector<QString> getOriginalCookieStringsByName(const QString &cookieName) const;
    QVector<QString> getOriginalCookieStringsByDomain(const QString &cookieDomain) const;
    QVector<QString> getOriginalCookieStringsByPath(const QString &cookiePath) const;
    QVector<QString> getOriginalCookieStringsByNameAndDomain(const QString &cookieName, const QString &cookieDomain) const;
    QVector<QString> getOriginalCookieStringsByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath) const;
    QVector<QString> getAllCookieStrings() const;

public slots:
    void runJavaScript(const QString &scriptSource);
    QVariant runJavaScriptSynchronous(const QString &scriptSource);

signals:
    // will be called during authentication phase
    // when a webengineview has been created, it can be attached to a widget using this signal
    // if not connected a new window with the webengineview will be shown
    void addWebView(QWidget *view);
    // will be called when the authentication phase is over
    // when the webview should be detached from a widget, this signal has to be connected
    // if not connected, the shown webengineview will closed
    void closed();
    // will be called, when the authenticated reached the point of a valid authenticated
    void authenticated(const QUrl &url);
    void authenticatedContent(const QUrl &url, const QByteArray &content);
    // will be called (if connected) if a custom authenticated check is needed
    // return true, if the authentication check was successful
    // otherwise, the authentication phase will just continue
    bool authenticationCheck(const QUrl &url);
    bool authenticationCheckContent(const QUrl &url, const QByteArray &content);
    // will be called, if there is a ssl certificate error
    // return true to ignore the certificate error, return false to cancel the connection
    bool certificateError(const QRestWrapperCertificateError &error);

signals:
    // will be called once the script has ended, which has been started by runJavaScript()
    void scriptEnded(const QVariant &result);

private slots:
    void checkUrlForAuthentication(const QUrl &url);
    void checkUrlForCustomAuthentication(const QUrl &url);
    void checkUrlForImplementedAuthentication(const QUrl &url);
    void urlCheckFinished(const QUrl &url);
    void initWebEngineView();
    void clientCertificateNeeded(QWebEngineClientCertificateSelection clientCertificateSelection);

private:
    void removeViewAfterAuthentication();
    void cleanupAfterAuthentication();

protected:
    QVector<QString> m_allowedUrlPatterns;
    QVector<QString> m_forbiddenUrlPatterns;

protected:
    QRestWrapperCookieJar *m_cookieJar;
    QRestWrapperUrlInterceptor *m_urlInterceptor;
    QRestWrapperView *m_view;
    TempWidget *m_tempWidget;
    QRestWrapperPage *m_page;
    QWebEngineCookieStore *m_cookieStore;

    QUrl m_authenticationTestUrl;
    QUrl m_applicationUrl;
    bool m_isAuthenticated;
    QString m_storagePath;
    bool m_alternateAuthenticationCheck;
    bool m_fetchContentOnAuthenticated;
    bool m_fetchContentOnAuthenticationCheck;

protected:
    QRestWrapperJavascriptInjector *m_javascriptInjector;
};
}  // namespace QtRestWrapper

#endif // QRESTWRAPPERURLCHECKER_H
