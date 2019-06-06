#ifndef TST_QRESTWRAPPERCOOKIEJARTEST_H
#define TST_QRESTWRAPPERCOOKIEJARTEST_H

#include <QObject>

#include "../QtRestWrapper/private/qrestwrappercookiejar.h"

namespace QtRestWrapper {

class QRestWrapperCookieJarTest : public QObject
{
    Q_OBJECT
public:
    QRestWrapperCookieJarTest(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void insertCookie_case();
    void updateCookie_case();
    void clearAllCookies_case();
    void deleteCookiesByNameAndDomainAndPath_case();
    void deleteCookiesByNameAndDomain_case();
    void deleteCookiesByName_case();
    void deleteCookiesByDomain_case();
    void deleteCookiesByPath_case();
    void getCookiesByNameAndDomainAndPath_case();
    void getCookiesByNameAndDomain_case();
    void getCookiesByName_case();
    void getCookiesByDomain_case();
    void getCookiesByPath_case();
    void cleanupTestCase();

private:
    QRestWrapperCookieJar *m_cookieJarInstance;
    QNetworkCookie m_testCookie;
    QNetworkCookie m_testCookie2;
    QNetworkCookie m_testCookie3;
    QNetworkCookie m_testCookie4;
    QNetworkCookie m_updatedCookie;

    static const short m_insertedCookieCount = 4;
};

}  // namespace QtRestWrapper

#endif // TST_QRESTWRAPPERCOOKIEJARTEST_H
