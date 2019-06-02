#include <QtTest>
#include <QCoreApplication>
#include <QMainWindow>
#include <QVBoxLayout>

// add necessary includes here
#include "../QtRestWrapper/qrestwrapper.h"
#include "../QtRestWrapper/qrestwrapper_p.h"

#include "tst_qrestwrapper.h"

using namespace QtRestWrapper;

QRestWrapperTest::QRestWrapperTest()
{

}

QRestWrapperTest::~QRestWrapperTest()
{
    m_restWrapperInstance->deleteLater();
    m_restWrapperInstance = nullptr;
    QVERIFY(m_restWrapperInstance == nullptr);
}

void QRestWrapperTest::initTestCase()
{
    m_restWrapperInstance = new QRestWrapper();
    QVERIFY(m_restWrapperInstance != nullptr);
    if(QMetaObject::invokeMethod(m_restWrapperInstance, "d_ptrFunc", Qt::DirectConnection, Q_RETURN_ARG(QRestWrapperPrivate *, m_restWrapperPrivateInstance))) {
        QVERIFY(m_restWrapperPrivateInstance != nullptr);
    } else {
        QVERIFY(false);
    }
}

class QRestWrapperPrivateTemp : public QRestWrapperPrivate {
public:
    QRestWrapperAuthenticator const * getAuthenticator() const {
        return &m_authenticator;
    }
    QRestWrapperNetworkHandler const * getNetworkHandler() const {
        return &m_networkHandler;
    }
};

class QRestWrapperAuthenticatorTemp : public QRestWrapperAuthenticator {
public:
    QString getStoragePath() const {
        return m_storagePath;
    }
    QUrl getTestUrl() const {
        return m_authenticationTestUrl;
    }
    QUrl getApplicationUrl() const {
        return m_applicationUrl;
    }
};

void QRestWrapperTest::setStoragePathTest()
{
    m_restWrapperInstance->setStoragePath("");
    QRestWrapperPrivateTemp *tempInstance = static_cast<QRestWrapperPrivateTemp *>(m_restWrapperPrivateInstance);
    QRestWrapperAuthenticator const * constAuthenticator = tempInstance->getAuthenticator();
    QRestWrapperAuthenticatorTemp const * tempAuthenticator = static_cast<QRestWrapperAuthenticatorTemp const *>(constAuthenticator);
    QCOMPARE(tempAuthenticator->getStoragePath(), qApp->applicationDirPath() + '/');
    m_restWrapperInstance->setStoragePath("C:\\temp\\");
    QCOMPARE(tempAuthenticator->getStoragePath(), "C:/temp/");
    m_restWrapperInstance->setStoragePath("cache");
    QCOMPARE(tempAuthenticator->getStoragePath(), qApp->applicationDirPath() + "/cache/");
}

void QRestWrapperTest::setAuthenticationTestUrlTest()
{
    QUrl testUrl("https://localhost:3000/protectedContent?ssoToken=(.*)");
    m_restWrapperInstance->setAuthenticationTestUrl(testUrl);
    QRestWrapperPrivateTemp *tempInstance = static_cast<QRestWrapperPrivateTemp *>(m_restWrapperPrivateInstance);
    QRestWrapperAuthenticator const * constAuthenticator = tempInstance->getAuthenticator();
    QRestWrapperAuthenticatorTemp const * tempAuthenticator = static_cast<QRestWrapperAuthenticatorTemp const *>(constAuthenticator);
    QCOMPARE(tempAuthenticator->getTestUrl(), testUrl);
}

void QRestWrapperTest::setApplicationUrlTest()
{
    QUrl applicationUrl("https://localhost:3000/protectedContent");
    m_restWrapperInstance->setApplicationUrl(applicationUrl);
    QRestWrapperPrivateTemp *tempInstance = static_cast<QRestWrapperPrivateTemp *>(m_restWrapperPrivateInstance);
    QRestWrapperAuthenticator const * constAuthenticator = tempInstance->getAuthenticator();
    QRestWrapperAuthenticatorTemp const * tempAuthenticator = static_cast<QRestWrapperAuthenticatorTemp const *>(constAuthenticator);
    QCOMPARE(tempAuthenticator->getApplicationUrl(), applicationUrl);
}

void QRestWrapperTest::authenticateCustomTest()
{
    connect(m_restWrapperInstance, &QRestWrapper::certificateError, [](const QRestWrapperCertificateError &error) {
        // ignore all errors related to self signed certificates
        return true;
    });
    //QEventLoop loop;
    QUrl *finalUrl = new QUrl();
    QMainWindow *newWindow = new QMainWindow();
    connect(m_restWrapperInstance, &QRestWrapper::authenticated, [&finalUrl, newWindow/*, &loop*/](const QUrl &url) {
        qDebug() << "authenticated.";
        //QCOMPARE(url, QUrl("https://localhost:3000/protectedContent"));
        //*finalUrl = url;
        //loop.quit();
        newWindow->close();
    });
    connect(m_restWrapperInstance, &QRestWrapper::authenticationCheckContent, [this](const QUrl &url, const QByteArray &content) {
        qDebug() << "current url: " << url;
        if(url.toString().startsWith("https://localhost:3001/simplesso/login")) {
            qDebug() << "incrementing i";
            m_ssoServerLoaded = true;
        }
        qDebug() << "content: " << content;
        if(m_ssoServerLoaded && url == QUrl("https://localhost:3000/protectedContent")) {
            qDebug() << "returning true";
            return true;
        } else if(content == "<html><head></head><body><h1>protected content</h1></body></html>" && url == QUrl("https://localhost:3000/protectedContent")) {
            qDebug() << "returning true";
            return true;
        }
        return false;
    });
    newWindow->setAttribute(Qt::WA_DeleteOnClose, true);
    newWindow->setLayout(new QVBoxLayout());
    connect(m_restWrapperInstance, &QRestWrapper::addWebView, [newWindow](QWidget *widget) {
        qDebug() << "adding web view";
        widget->setParent(newWindow);
        newWindow->layout()->addWidget(widget);
        newWindow->show();
    });
    m_restWrapperInstance->authenticate();
    //loop.exec();
    qDebug() << __FUNCTION__;
    //QCOMPARE(*finalUrl, QUrl("https://localhost:3000/protectedContent"));
}

void QRestWrapperTest::authenticateCustomWithoutWindowTest()
{
    connect(m_restWrapperInstance, &QRestWrapper::certificateError, [](const QRestWrapperCertificateError &error) {
        // ignore all errors related to self signed certificates
        return true;
    });
    QEventLoop loop;
    QUrl *finalUrl = new QUrl();
    connect(m_restWrapperInstance, &QRestWrapper::authenticated, [&finalUrl, this, &loop](const QUrl &url) {
        qDebug() << "authenticated.";
        //QCOMPARE(url, QUrl("https://localhost:3000/protectedContent"));
        //*finalUrl = url;
        loop.quit();
        m_viewWidget->close();
    });
    connect(m_restWrapperInstance, &QRestWrapper::authenticationCheckContent, [this](const QUrl &url, const QByteArray &content) {
        qDebug() << "current url: " << url;
        if(url.toString().startsWith("https://localhost:3001/simplesso/login")) {
            qDebug() << "incrementing i";
            m_ssoServerLoaded = true;
        }
        qDebug() << "content: " << content;
        if(m_ssoServerLoaded && url == QUrl("https://localhost:3000/protectedContent")) {
            qDebug() << "returning true";
            return true;
        } else if(content == "<html><head></head><body><h1>protected content</h1></body></html>" && url == QUrl("https://localhost:3000/protectedContent")) {
            qDebug() << "returning true";
            return true;
        }
        return false;
    });
    connect(m_restWrapperInstance, &QRestWrapper::addWebView, [this](QWidget *widget) {
        qDebug() << "adding web view";
        m_viewWidget = widget;
        m_viewWidget->setAttribute(Qt::WA_DeleteOnClose, true);
        m_viewWidget->setParent(nullptr);
        m_viewWidget->setWindowFlag(Qt::Window, true);
        widget->setFixedWidth(600);
        widget->setFixedHeight(400);
        widget->show();
    });
    m_restWrapperInstance->authenticate();
    loop.exec();
    qDebug() << __FUNCTION__;
    //QCOMPARE(*finalUrl, QUrl("https://localhost:3000/protectedContent"));
}

void QRestWrapperTest::authenticateImplementedTest()
{
    QEventLoop loop;
    QUrl *finalUrl = new QUrl();
    connect(m_restWrapperInstance, &QRestWrapper::authenticated, [&finalUrl, &loop](const QUrl &url) {
        qDebug() << "authenticated.";
        QVERIFY(url.toString().startsWith(QUrl("https://localhost:3000/protectedContent?ssoToken=").toString()));
        *finalUrl = url;
        loop.quit();
    });
    m_restWrapperInstance->authenticate();
    loop.exec();
    QCOMPARE(*finalUrl, QUrl("https://localhost:3000/protectedContent"));
}

void QRestWrapperTest::cleanupTestCase()
{

}

//QTEST_MAIN(QRestWrapperTest)

//#include "tst_QRestWrapper.moc"
