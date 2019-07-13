#ifndef QRESTWRAPPERCOOKIEJAR_H
#define QRESTWRAPPERCOOKIEJAR_H

#include <QMap>
#include <QPair>
#include <QString>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QVector>

#include <tuple>

#include "../qtrestwrapper_global.h"

template<class _This, class... _Rest>
using QTuple = typename std::tuple<_This, _Rest...>;

namespace QtRestWrapper {

class QRestWrapperCookieJarTest;

class QTRESTWRAPPERSHARED_EXPORT QRestWrapperCookieJar : public QNetworkCookieJar
{
public:
    explicit QRestWrapperCookieJar(QObject *parent = nullptr);
    virtual ~QRestWrapperCookieJar() override;

public:
    virtual bool insertCookie(const QNetworkCookie &cookie) override;
    virtual bool deleteCookie(const QNetworkCookie &cookie) override;
    virtual bool updateCookie(const QNetworkCookie &cookie) override;
    virtual bool validateCookie(const QNetworkCookie &cookie, const QUrl &url) const override;

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

private:
    QMap<QTuple<QString/*name*/, QString/*domain*/, QString/*path*/>, QNetworkCookie> m_cookiesBackup;
    QMap<QTuple<QString/*name*/, QString/*domain*/, QString/*path*/>, QString> m_originalCookieStrings;

private:
    friend class QRestWrapperCookieJarTest;
};

} // namespace QtRestWrapper

#endif // QRESTWRAPPERCOOKIEJAR_H
