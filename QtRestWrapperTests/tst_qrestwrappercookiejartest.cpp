#include "tst_qrestwrappercookiejartest.h"

#include <QtTest>

using namespace QtRestWrapper;

QRestWrapperCookieJarTest::QRestWrapperCookieJarTest(QObject *parent) : QObject(parent)
{
    m_testCookie.setName("testCookie");
    m_testCookie.setPath("/");
    m_testCookie.setValue("testValue");
    m_testCookie.setDomain("example.com");
    m_testCookie.setSecure(false);
    m_testCookie.setHttpOnly(true);
    QDateTime date = QDateTime::currentDateTime();
    date.addDays(100); // add enough days to be a valid cookie
    m_testCookie.setExpirationDate(date);

    m_testCookie2.setName("testCookie2");
    m_testCookie2.setPath("/");
    m_testCookie2.setValue("testValue2");
    m_testCookie2.setDomain("example.com");
    m_testCookie2.setSecure(false);
    m_testCookie2.setHttpOnly(true);
    m_testCookie2.setExpirationDate(date);

    m_testCookie3.setName("testCookie3");
    m_testCookie3.setPath("/path");
    m_testCookie3.setValue("testValue3");
    m_testCookie3.setDomain("example.com");
    m_testCookie3.setSecure(false);
    m_testCookie3.setHttpOnly(true);
    m_testCookie3.setExpirationDate(date);

    m_testCookie4.setName("testCookie2");
    m_testCookie4.setPath("/");
    m_testCookie4.setValue("testValue4");
    m_testCookie4.setDomain("contoso.com");
    m_testCookie4.setSecure(false);
    m_testCookie4.setHttpOnly(true);
    m_testCookie4.setExpirationDate(date);

    m_updatedCookie = m_testCookie;
    m_updatedCookie.setValue("changedValue");
}

void QRestWrapperCookieJarTest::initTestCase()
{
    m_cookieJarInstance = new QRestWrapperCookieJar();
    QVERIFY(m_cookieJarInstance != nullptr);
}

void QRestWrapperCookieJarTest::insertCookie_case()
{
    m_cookieJarInstance->insertCookie(m_testCookie);
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), 1);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), 1);
    QTuple<QString, QString, QString> key(m_testCookie.name(), m_testCookie.domain(), m_testCookie.path());
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key], m_testCookie);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key], m_testCookie.toRawForm());

    m_cookieJarInstance->insertCookie(m_testCookie2);
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), 2);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), 2);
    QTuple<QString, QString, QString> key2(m_testCookie2.name(), m_testCookie2.domain(), m_testCookie2.path());
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key2], m_testCookie2);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key2], m_testCookie2.toRawForm());

    m_cookieJarInstance->insertCookie(m_testCookie3);
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), 3);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), 3);
    QTuple<QString, QString, QString> key3(m_testCookie3.name(), m_testCookie3.domain(), m_testCookie3.path());
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key3], m_testCookie3);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key3], m_testCookie3.toRawForm());

    m_cookieJarInstance->insertCookie(m_testCookie4);
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), 4);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), 4);
    QTuple<QString, QString, QString> key4(m_testCookie4.name(), m_testCookie4.domain(), m_testCookie4.path());
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key4], m_testCookie4);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key4], m_testCookie4.toRawForm());
}

void QRestWrapperCookieJarTest::updateCookie_case()
{
    clearAllCookies_case();
    insertCookie_case();

    m_cookieJarInstance->updateCookie(m_updatedCookie);

    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), m_insertedCookieCount);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), m_insertedCookieCount);
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.first(), m_updatedCookie);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.first(), m_updatedCookie.toRawForm());
}

void QRestWrapperCookieJarTest::clearAllCookies_case()
{
    m_cookieJarInstance->clearAllCookies();

    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), 0);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), 0);
}

void QRestWrapperCookieJarTest::deleteCookiesByNameAndDomainAndPath_case()
{
    clearAllCookies_case();
    insertCookie_case();

    m_cookieJarInstance->deleteCookiesByNameAndDomainAndPath(m_testCookie4.name(), m_testCookie4.domain(), m_testCookie4.path());

    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), m_insertedCookieCount - 1);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), m_insertedCookieCount - 1);
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[m_cookieJarInstance->m_cookiesBackup.keys()[0]], m_testCookie);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[m_cookieJarInstance->m_originalCookieStrings.keys()[0]], m_testCookie.toRawForm());
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[m_cookieJarInstance->m_cookiesBackup.keys()[1]], m_testCookie2);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[m_cookieJarInstance->m_originalCookieStrings.keys()[1]], m_testCookie2.toRawForm());
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[m_cookieJarInstance->m_cookiesBackup.keys()[2]], m_testCookie3);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[m_cookieJarInstance->m_originalCookieStrings.keys()[2]], m_testCookie3.toRawForm());

    m_cookieJarInstance->deleteCookiesByNameAndDomainAndPath(m_testCookie3.name(), m_testCookie3.domain(), m_testCookie3.path());

    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), m_insertedCookieCount - 2);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), m_insertedCookieCount - 2);
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[m_cookieJarInstance->m_cookiesBackup.keys()[0]], m_testCookie);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[m_cookieJarInstance->m_originalCookieStrings.keys()[0]], m_testCookie.toRawForm());
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[m_cookieJarInstance->m_cookiesBackup.keys()[1]], m_testCookie2);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[m_cookieJarInstance->m_originalCookieStrings.keys()[1]], m_testCookie2.toRawForm());

    m_cookieJarInstance->deleteCookiesByNameAndDomainAndPath(m_testCookie2.name(), m_testCookie2.domain(), m_testCookie2.path());

    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), m_insertedCookieCount - 3);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), m_insertedCookieCount - 3);
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[m_cookieJarInstance->m_cookiesBackup.keys()[0]], m_testCookie);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[m_cookieJarInstance->m_originalCookieStrings.keys()[0]], m_testCookie.toRawForm());

    m_cookieJarInstance->deleteCookiesByNameAndDomainAndPath(m_testCookie.name(), m_testCookie.domain(), m_testCookie.path());

    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), 0);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), 0);
}

void QRestWrapperCookieJarTest::deleteCookiesByNameAndDomain_case()
{
    clearAllCookies_case();
    insertCookie_case();

    QTuple<QString, QString, QString> key3(m_testCookie3.name(), m_testCookie3.domain(), m_testCookie3.path());
    QTuple<QString, QString, QString> key2(m_testCookie2.name(), m_testCookie2.domain(), m_testCookie2.path());
    QTuple<QString, QString, QString> key(m_testCookie.name(), m_testCookie.domain(), m_testCookie.path());

    m_cookieJarInstance->deleteCookiesByNameAndDomain(m_testCookie4.name(), m_testCookie4.domain());

    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), m_insertedCookieCount - 1);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), m_insertedCookieCount - 1);
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key3], m_testCookie3);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key3], m_testCookie3.toRawForm());
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key2], m_testCookie2);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key2], m_testCookie2.toRawForm());
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key], m_testCookie);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key], m_testCookie.toRawForm());

    m_cookieJarInstance->deleteCookiesByNameAndDomain(m_testCookie3.name(), m_testCookie3.domain());

    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), m_insertedCookieCount - 2);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), m_insertedCookieCount - 2);
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key2], m_testCookie2);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key2], m_testCookie2.toRawForm());
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key], m_testCookie);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key], m_testCookie.toRawForm());

    m_cookieJarInstance->deleteCookiesByNameAndDomain(m_testCookie2.name(), m_testCookie2.domain());

    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), m_insertedCookieCount - 3);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), m_insertedCookieCount - 3);
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key], m_testCookie);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key], m_testCookie.toRawForm());

    m_cookieJarInstance->deleteCookiesByNameAndDomain(m_testCookie.name(), m_testCookie.domain());

    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), m_insertedCookieCount - 4);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), m_insertedCookieCount - 4);
}

void QRestWrapperCookieJarTest::deleteCookiesByName_case()
{
    clearAllCookies_case();
    insertCookie_case();

    QTuple<QString, QString, QString> key3(m_testCookie3.name(), m_testCookie3.domain(), m_testCookie3.path());
    QTuple<QString, QString, QString> key2(m_testCookie2.name(), m_testCookie2.domain(), m_testCookie2.path());
    QTuple<QString, QString, QString> key(m_testCookie.name(), m_testCookie.domain(), m_testCookie.path());

    m_cookieJarInstance->deleteCookiesByName(m_testCookie4.name());

    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), m_insertedCookieCount - 2);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), m_insertedCookieCount - 2);
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key3], m_testCookie3);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key3], m_testCookie3.toRawForm());
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key], m_testCookie);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key], m_testCookie.toRawForm());

    m_cookieJarInstance->deleteCookiesByNameAndDomain(m_testCookie3.name(), m_testCookie3.domain());

    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), m_insertedCookieCount - 3);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), m_insertedCookieCount - 3);
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key], m_testCookie);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key], m_testCookie.toRawForm());

    m_cookieJarInstance->deleteCookiesByNameAndDomain(m_testCookie.name(), m_testCookie.domain());

    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), m_insertedCookieCount - 4);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), m_insertedCookieCount - 4);
}

void QRestWrapperCookieJarTest::deleteCookiesByDomain_case()
{
    clearAllCookies_case();
    insertCookie_case();

    QTuple<QString, QString, QString> key3(m_testCookie3.name(), m_testCookie3.domain(), m_testCookie3.path());
    QTuple<QString, QString, QString> key2(m_testCookie2.name(), m_testCookie2.domain(), m_testCookie2.path());
    QTuple<QString, QString, QString> key(m_testCookie.name(), m_testCookie.domain(), m_testCookie.path());

    m_cookieJarInstance->deleteCookiesByDomain(m_testCookie4.domain());

    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), m_insertedCookieCount - 1);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), m_insertedCookieCount - 1);
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key3], m_testCookie3);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key3], m_testCookie3.toRawForm());
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key2], m_testCookie2);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key2], m_testCookie2.toRawForm());
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key], m_testCookie);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key], m_testCookie.toRawForm());

    m_cookieJarInstance->deleteCookiesByDomain(m_testCookie3.domain());

    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), m_insertedCookieCount - 4);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), m_insertedCookieCount - 4);
}

void QRestWrapperCookieJarTest::deleteCookiesByPath_case()
{
    clearAllCookies_case();
    insertCookie_case();

    QTuple<QString, QString, QString> key4(m_testCookie4.name(), m_testCookie4.domain(), m_testCookie4.path());
    QTuple<QString, QString, QString> key2(m_testCookie2.name(), m_testCookie2.domain(), m_testCookie2.path());
    QTuple<QString, QString, QString> key(m_testCookie.name(), m_testCookie.domain(), m_testCookie.path());

    m_cookieJarInstance->deleteCookiesByPath(m_testCookie3.path());

    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), m_insertedCookieCount - 1);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), m_insertedCookieCount - 1);
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key4], m_testCookie4);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key4], m_testCookie4.toRawForm());
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key2], m_testCookie2);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key2], m_testCookie2.toRawForm());
    QCOMPARE(m_cookieJarInstance->m_cookiesBackup[key], m_testCookie);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings[key], m_testCookie.toRawForm());

    m_cookieJarInstance->deleteCookiesByPath(m_testCookie4.path());

    QCOMPARE(m_cookieJarInstance->m_cookiesBackup.size(), m_insertedCookieCount - 4);
    QCOMPARE(m_cookieJarInstance->m_originalCookieStrings.size(), m_insertedCookieCount - 4);
}

void QRestWrapperCookieJarTest::getCookiesByNameAndDomainAndPath_case()
{
    clearAllCookies_case();
    insertCookie_case();
}

void QRestWrapperCookieJarTest::getCookiesByNameAndDomain_case()
{
    clearAllCookies_case();
    insertCookie_case();
}

void QRestWrapperCookieJarTest::getCookiesByName_case()
{
    clearAllCookies_case();
    insertCookie_case();
}

void QRestWrapperCookieJarTest::getCookiesByDomain_case()
{
    clearAllCookies_case();
    insertCookie_case();
}

void QRestWrapperCookieJarTest::getCookiesByPath_case()
{
    clearAllCookies_case();
    insertCookie_case();
}

void QRestWrapperCookieJarTest::cleanupTestCase()
{
    m_cookieJarInstance->deleteLater();
    m_cookieJarInstance = nullptr;
    QVERIFY(m_cookieJarInstance == nullptr);
}
