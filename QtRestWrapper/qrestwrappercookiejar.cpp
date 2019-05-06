#include "qrestwrappercookiejar.h"

#include <QNetworkCookie>
#include <QVector>

using QtRestWrapper::QRestWrapperCookieJar;

QRestWrapperCookieJar::QRestWrapperCookieJar(QObject *parent) : QNetworkCookieJar(parent)
{

}

bool QRestWrapperCookieJar::insertCookie(const QNetworkCookie &cookie)
{
    this->m_cookiesBackup[QPair<QString, QString>(cookie.name(), cookie.domain())] = cookie;
    this->m_originalCookieStrings[std::make_tuple<QString, QString, QString>(cookie.name(), cookie.domain(), cookie.path())] = cookie.toRawForm();
    return QNetworkCookieJar::insertCookie(cookie);
}

bool QRestWrapperCookieJar::deleteCookie(const QNetworkCookie &cookie)
{
    return QNetworkCookieJar::deleteCookie(cookie);
}

bool QRestWrapperCookieJar::updateCookie(const QNetworkCookie &cookie)
{
    this->m_cookiesBackup[QPair<QString, QString>(cookie.name(), cookie.domain())] = cookie;
    return QNetworkCookieJar::updateCookie(cookie);
}

bool QRestWrapperCookieJar::validateCookie(const QNetworkCookie &cookie, const QUrl &url) const
{
    return QNetworkCookieJar::validateCookie(cookie, url);
}

void QRestWrapperCookieJar::deleteCookiesByName(const QString &cookieName)
{
    for (auto it = m_cookiesBackup.begin(); it != m_cookiesBackup.end();) {
        if(it->name() == cookieName) {
            it = m_cookiesBackup.erase(it);
        } else {
            ++it;
        }
    }
}

void QRestWrapperCookieJar::deleteCookiesByDomain(const QString &cookieDomain)
{
    for (auto it = m_cookiesBackup.begin(); it != m_cookiesBackup.end();) {
        if(it->domain() == cookieDomain) {
            it = m_cookiesBackup.erase(it);
        } else {
            ++it;
        }
    }
}

void QRestWrapperCookieJar::deleteCookiesByPath(const QString &cookiePath)
{
    for (auto it = m_cookiesBackup.begin(); it != m_cookiesBackup.end();) {
        if(it->path() == cookiePath) {
            it = m_cookiesBackup.erase(it);
        } else {
            ++it;
        }
    }
}

void QRestWrapperCookieJar::deleteCookiesByNameAndDomain(const QString &cookieName, const QString &cookieDomain)
{
    for (auto it = m_cookiesBackup.begin(); it != m_cookiesBackup.end();) {
        if(it->name() == cookieName && it->domain() == cookieDomain) {
            it = m_cookiesBackup.erase(it);
        } else {
            ++it;
        }
    }
}

void QRestWrapperCookieJar::deleteCookiesByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath)
{
    for (auto it = m_cookiesBackup.begin(); it != m_cookiesBackup.end();) {
        if(it->name() == cookieName && it->domain() == cookieDomain && it->path() == cookiePath) {
            it = m_cookiesBackup.erase(it);
        } else {
            ++it;
        }
    }
}

void QRestWrapperCookieJar::clearAllCookies()
{
    m_cookiesBackup.clear();
}

QVector<QNetworkCookie> QRestWrapperCookieJar::getCookiesByName(const QString &cookieName) const
{
    QVector<QNetworkCookie> cookies;
    for(auto &cookie : m_cookiesBackup) {
        if(cookie.name() == cookieName) {
            cookies.push_back(cookie);
        }
    }
    return cookies;
}

QVector<QNetworkCookie> QRestWrapperCookieJar::getCookiesByDomain(const QString &cookieDomain) const
{
    QVector<QNetworkCookie> cookies;
    for(auto &cookie : m_cookiesBackup) {
        if(cookie.domain() == cookieDomain) {
            cookies.push_back(cookie);
        }
    }
    return cookies;
}

QVector<QNetworkCookie> QRestWrapperCookieJar::getCookiesByPath(const QString &cookiePath) const
{
    QVector<QNetworkCookie> cookies;
    for(auto &cookie : m_cookiesBackup) {
        if(cookie.path() == cookiePath) {
            cookies.push_back(cookie);
        }
    }
    return cookies;
}

QVector<QNetworkCookie> QRestWrapperCookieJar::getCookiesByNameAndDomain(const QString &cookieName, const QString &cookieDomain) const
{
    QVector<QNetworkCookie> cookies;
    for(auto &cookie : m_cookiesBackup) {
        if(cookie.name() == cookieName && cookie.domain() == cookieDomain) {
            cookies.push_back(cookie);
        }
    }
    return cookies;
}

QVector<QNetworkCookie> QRestWrapperCookieJar::getCookiesByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath) const
{
    QVector<QNetworkCookie> cookies;
    for(auto &cookie : m_cookiesBackup) {
        if(cookie.name() == cookieName && cookie.domain() == cookieDomain && cookie.path() == cookiePath) {
            cookies.push_back(cookie);
        }
    }
    return cookies;
}

QMap<QPair<QString, QString>, QNetworkCookie> QRestWrapperCookieJar::getAllCookies() const
{
    return m_cookiesBackup;
}

QVector<QString> QRestWrapperCookieJar::getOriginalCookieStringsByName(const QString &cookieName) const
{
    QVector<QString> cookieStrings;
    for(auto it = m_originalCookieStrings.begin(); it != m_originalCookieStrings.end();) {
        if(std::get<0>(it.key()) == cookieName) {
            cookieStrings.push_back(*it);
        }
    }
    return cookieStrings;
}

QVector<QString> QRestWrapperCookieJar::getOriginalCookieStringsByDomain(const QString &cookieDomain) const
{
    QVector<QString> cookieStrings;
    for(auto it = m_originalCookieStrings.begin(); it != m_originalCookieStrings.end();) {
        if(std::get<1>(it.key()) == cookieDomain) {
            cookieStrings.push_back(*it);
        }
    }
    return cookieStrings;
}

QVector<QString> QRestWrapperCookieJar::getOriginalCookieStringsByPath(const QString &cookiePath) const
{
    QVector<QString> cookieStrings;
    for(auto it = m_originalCookieStrings.begin(); it != m_originalCookieStrings.end();) {
        if(std::get<2>(it.key()) == cookiePath) {
            cookieStrings.push_back(*it);
        }
    }
    return cookieStrings;
}

QVector<QString> QRestWrapperCookieJar::getOriginalCookieStringsByNameAndDomain(const QString &cookieName, const QString &cookieDomain) const
{
    QVector<QString> cookieStrings;
    for(auto it = m_originalCookieStrings.begin(); it != m_originalCookieStrings.end();) {
        if(std::get<0>(it.key()) == cookieName && std::get<1>(it.key()) == cookieDomain) {
            cookieStrings.push_back(*it);
        }
    }
    return cookieStrings;
}

QVector<QString> QRestWrapperCookieJar::getOriginalCookieStringsByNameAndDomainAndPath(const QString &cookieName, const QString &cookieDomain, const QString &cookiePath) const
{
    QVector<QString> cookieStrings;
    for(auto it = m_originalCookieStrings.begin(); it != m_originalCookieStrings.end();) {
        if(std::get<0>(it.key()) == cookieName && std::get<1>(it.key()) == cookieDomain && std::get<2>(it.key()) == cookiePath) {
            cookieStrings.push_back(*it);
        }
    }
    return cookieStrings;
}

QVector<QString> QRestWrapperCookieJar::getAllCookieStrings() const
{
    return m_originalCookieStrings.values().toVector();
}
