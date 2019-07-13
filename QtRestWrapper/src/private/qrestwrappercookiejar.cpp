#include "../../include/private/qrestwrappercookiejar.h"

#include <QNetworkCookie>
#include <QVector>

using QtRestWrapper::QRestWrapperCookieJar;

QRestWrapperCookieJar::QRestWrapperCookieJar(QObject *parent) : QNetworkCookieJar(parent)
{

}

QRestWrapperCookieJar::~QRestWrapperCookieJar()
{
}

bool QRestWrapperCookieJar::insertCookie(const QNetworkCookie &cookie)
{
    if(getCookiesByNameAndDomainAndPath(cookie.name(), cookie.domain(), cookie.path()).size() <= 0) {
        this->m_cookiesBackup[QTuple<QString/*name*/, QString/*domain*/, QString/*path*/>(cookie.name(), cookie.domain(), cookie.path())] = cookie;
        this->m_originalCookieStrings[std::make_tuple<QString, QString, QString>(cookie.name(), cookie.domain(), cookie.path())] = cookie.toRawForm();
    }
    return QNetworkCookieJar::insertCookie(cookie);
}

bool QRestWrapperCookieJar::deleteCookie(const QNetworkCookie &cookie)
{
    return QNetworkCookieJar::deleteCookie(cookie);
}

bool QRestWrapperCookieJar::updateCookie(const QNetworkCookie &cookie)
{
    if(getCookiesByNameAndDomainAndPath(cookie.name(), cookie.domain(), cookie.path()).size() > 0) {
        this->m_cookiesBackup[QTuple<QString/*name*/, QString/*domain*/, QString/*path*/>(cookie.name(), cookie.domain(), cookie.path())] = cookie;
        this->m_originalCookieStrings[std::make_tuple<QString, QString, QString>(cookie.name(), cookie.domain(), cookie.path())] = cookie.toRawForm();
    }
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

    for (auto it = m_originalCookieStrings.begin(); it != m_originalCookieStrings.end();) {
        if(std::get<0>(it.key()) == cookieName) {
            it = m_originalCookieStrings.erase(it);
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

    for (auto it = m_originalCookieStrings.begin(); it != m_originalCookieStrings.end();) {
        if(std::get<1>(it.key()) == cookieDomain) {
            it = m_originalCookieStrings.erase(it);
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

    for (auto it = m_originalCookieStrings.begin(); it != m_originalCookieStrings.end();) {
        if(std::get<2>(it.key()) == cookiePath) {
            it = m_originalCookieStrings.erase(it);
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

    for (auto it = m_originalCookieStrings.begin(); it != m_originalCookieStrings.end();) {
        if(std::get<0>(it.key()) == cookieName && std::get<1>(it.key()) == cookieDomain) {
            it = m_originalCookieStrings.erase(it);
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

    for (auto it = m_originalCookieStrings.begin(); it != m_originalCookieStrings.end();) {
        if(std::get<0>(it.key()) == cookieName && std::get<1>(it.key()) == cookieDomain && std::get<2>(it.key()) == cookiePath) {
            it = m_originalCookieStrings.erase(it);
        } else {
            ++it;
        }
    }
}

void QRestWrapperCookieJar::clearAllCookies()
{
    m_cookiesBackup.clear();
    m_originalCookieStrings.clear();
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

QVector<QNetworkCookie> QRestWrapperCookieJar::getAllCookies() const
{
    return m_cookiesBackup.values().toVector();
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
