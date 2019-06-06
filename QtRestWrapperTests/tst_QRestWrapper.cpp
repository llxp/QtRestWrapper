#include <QtTest>
#include <QCoreApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWebEngineProfile>
#include <QTableWidget>
#include <QDialog>
#include <QPushButton>

// add necessary includes here
#include "../QtRestWrapper/qrestwrapper.h"
#include "../QtRestWrapper/private/qrestwrapper_p.h"

#include "tst_qrestwrapper.h"

using namespace QtRestWrapper;

QRestWrapperTest::QRestWrapperTest()
{

}

QRestWrapperTest::~QRestWrapperTest()
{
}

void QRestWrapperTest::webengineTest()
{
    QEventLoop loop;
    QRestWrapperUrlInterceptor *interceptor = new QRestWrapperUrlInterceptor(nullptr, nullptr);
    QRestWrapperView *view = new QRestWrapperView();
    view->setPage(new QRestWrapperPage(view));
    qobject_cast<QRestWrapperPage *>(view->page())->setRequestInterceptor(interceptor);
    connect(qobject_cast<QRestWrapperPage *>(view->page()), &QRestWrapperPage::selectClientCertificate, this, [](QWebEngineClientCertificateSelection clientCertificateSelection) {
        int i = 0;
        QDialog *certSelectionDialog = new QDialog();
        certSelectionDialog->setAttribute( Qt::WA_DeleteOnClose, true );
        QTableWidget *certList = new QTableWidget();
        certList->setRowCount(clientCertificateSelection.certificates().size());
        certList->setColumnCount(4);
        certList->setHorizontalHeaderLabels(QStringList({"issuer", "subject", "activation Date", "expiry Date"}));
        for(QSslCertificate cert : clientCertificateSelection.certificates()) {
            QString version = cert.version();
            QString issuerDisplayName = cert.issuerDisplayName();
            QString subjectDisplayName = cert.subjectDisplayName();
            QDateTime activationDate = cert.effectiveDate();
            QDateTime expiryDate = cert.expiryDate();
            QTableWidgetItem *issuerDisplayNameItem = new QTableWidgetItem(issuerDisplayName);
            issuerDisplayNameItem->setFlags(issuerDisplayNameItem->flags() ^ Qt::ItemIsEditable);
            certList->setItem(i, 0, issuerDisplayNameItem);
            QTableWidgetItem *subjectDisplayNameItem = new QTableWidgetItem(subjectDisplayName);
            subjectDisplayNameItem->setFlags(subjectDisplayNameItem->flags() ^ Qt::ItemIsEditable);
            certList->setItem(i, 1, subjectDisplayNameItem);
            QTableWidgetItem *activationDateItem = new QTableWidgetItem(activationDate.toString());
            activationDateItem->setFlags(activationDateItem->flags() ^ Qt::ItemIsEditable);
            certList->setItem(i, 2, activationDateItem);
            QTableWidgetItem *expiryDateItem = new QTableWidgetItem(expiryDate.toString());
            expiryDateItem->setFlags(expiryDateItem->flags() ^ Qt::ItemIsEditable);
            certList->setItem(i, 3, expiryDateItem);
            ++i;
        }
        QModelIndex selectedIndex;
        QPushButton *okButton = new QPushButton(certSelectionDialog);
        certList->connect(certList, &QTableWidget::clicked, [&selectedIndex, okButton](const QModelIndex &index) {
            selectedIndex = index;
            okButton->setDisabled(false);
        });
        certList->connect(certList, &QTableWidget::doubleClicked, [certSelectionDialog, &clientCertificateSelection](const QModelIndex &index) {
            clientCertificateSelection.select(clientCertificateSelection.certificates().at(index.row()));
            certSelectionDialog->close();
            certSelectionDialog->deleteLater();
        });
        certSelectionDialog->setLayout(new QVBoxLayout(certSelectionDialog));
        certSelectionDialog->layout()->addWidget(certList);
        okButton->connect(okButton, &QPushButton::clicked, [certSelectionDialog, &clientCertificateSelection, &selectedIndex]() {
            clientCertificateSelection.select(clientCertificateSelection.certificates().at(selectedIndex.row()));
            certSelectionDialog->close();
            certSelectionDialog->deleteLater();
        });
        okButton->setDisabled(true);
        okButton->setText("Ok");
        okButton->setFixedWidth(100);
        okButton->setFixedHeight(50);
        certSelectionDialog->connect(certSelectionDialog, &QDialog::accepted, [certSelectionDialog, &clientCertificateSelection, &selectedIndex]() {
            clientCertificateSelection.select(clientCertificateSelection.certificates().at(selectedIndex.row()));
            certSelectionDialog->deleteLater();
        });
        certSelectionDialog->connect(certSelectionDialog, &QDialog::rejected, [certSelectionDialog]() {
            certSelectionDialog->deleteLater();
        });
        certSelectionDialog->layout()->addWidget(okButton);
        certSelectionDialog->exec();
    }, Qt::QueuedConnection);
    view->setUrl(QUrl("https://localhost:3000/protectedContent"));
    connect(qobject_cast<QRestWrapperPage *>(view->page()), &QRestWrapperPage::certificateErrorSignal, [](const QRestWrapperCertificateError &error) {
        return true;
    });
    connect(view->tempWidget(), &TempWidget::closedEvent, [view, &loop]() {
        qDebug() << "closed.";
        view->deleteLater();
        loop.quit();
    });
    connect(interceptor, &QRestWrapperUrlInterceptor::checkAuthenticationPage, this, [this, view](const QUrl &url) {
        auto func = [this](const QUrl &url) -> bool {
            if(url.toString().startsWith("https://localhost:3001/simplesso/login")) {
            qDebug() << "incrementing i";
            m_ssoServerLoaded = true;
        }
        if(m_ssoServerLoaded && url == QUrl("https://localhost:3000/protectedContent")) {
            qDebug() << "returning true";
            return true;
        }
        return false;
        };
        if(func(url)) {
            view->close();
        }
    }, Qt::QueuedConnection);
    view->show();
    loop.exec();
}

void QRestWrapperTest::authenticatorTest()
{
    m_ssoServerLoaded = false;
    QEventLoop loop;
    QRestWrapperAuthenticator *authenticator = new QRestWrapperAuthenticator();
    authenticator->setAlternateAuthenticationCheck(true);
    authenticator->setApplicationUrl(QUrl("https://localhost:3000/protectedContent"));
    authenticator->setFetchContentOnAuthenticationCheck(true);
    authenticator->setStoragePath("cache");
    connect(authenticator, &QRestWrapperAuthenticator::closed, [&loop, authenticator]() {
        qDebug() << "closed.";
        authenticator->deleteLater();
        loop.quit();
    });
    connect(authenticator, &QRestWrapperAuthenticator::certificateError, [](const QRestWrapperCertificateError &error) {
        // ignore all errors related to self signed certificates
        return true;
    });
    QWidget *tempWidget = new QWidget();
    connect(authenticator, &QRestWrapperAuthenticator::addWebView, [&tempWidget](QWidget *widget) {
        tempWidget->deleteLater();
        tempWidget = nullptr;
        tempWidget = widget;
        widget->resize(640, 480);
        widget->show();
    });
    connect(authenticator, &QRestWrapperAuthenticator::authenticated, [&tempWidget](const QUrl &url) {
        qDebug() << "authenticated.";
        if(tempWidget != nullptr) {
            tempWidget->close();
        } else {
            qDebug() << "tempWidget is nullptr";
        }
    });
    connect(authenticator, &QRestWrapperAuthenticator::authenticationCheckContent, [this](const QUrl &url, const QByteArray &content) {
        qDebug() << "current url: " << url;
        if(url.toString().startsWith("https://localhost:3001/simplesso/login")) {
            qDebug() << "incrementing i";
            m_ssoServerLoaded = true;
        }
        if(m_ssoServerLoaded && url == QUrl("https://localhost:3000/protectedContent")) {
            qDebug() << "returning true";
            return true;
        } else if(content == "<html><head></head><body><h1>protected content</h1></body></html>" && url == QUrl("https://localhost:3000/protectedContent")) {
            qDebug() << "returning true";
            return true;
        }
        return false;
    });

    authenticator->authenticate();
    loop.exec();
    m_ssoServerLoaded = false;
}

void QRestWrapperTest::authenticatorTest2()
{
    m_ssoServerLoaded = false;
    QEventLoop loop;
    QRestWrapperAuthenticator *authenticator = new QRestWrapperAuthenticator();
    authenticator->setAlternateAuthenticationCheck(true);
    authenticator->setApplicationUrl(QUrl("https://localhost:3000/protectedContent"));
    authenticator->setFetchContentOnAuthenticationCheck(true);
    authenticator->setStoragePath("cache");
    connect(authenticator, &QRestWrapperAuthenticator::closed, [&loop, authenticator]() {
        qDebug() << "closed.";
        authenticator->deleteLater();
        loop.quit();
    });
    connect(authenticator, &QRestWrapperAuthenticator::certificateError, [](const QRestWrapperCertificateError &error) {
        // ignore all errors related to self signed certificates
        return true;
    });
    QWidget *tempWidget = new QWidget();
    connect(authenticator, &QRestWrapperAuthenticator::addWebView, [&tempWidget](QWidget *widget) {
        tempWidget->deleteLater();
        tempWidget = nullptr;
        tempWidget = widget;
        widget->resize(640, 480);
        widget->show();
    });
    connect(authenticator, &QRestWrapperAuthenticator::authenticated, [&tempWidget](const QUrl &url) {
        qDebug() << "authenticated.";
        if(tempWidget != nullptr) {
            tempWidget->close();
        } else {
            qDebug() << "tempWidget is nullptr";
        }
    });
    connect(authenticator, &QRestWrapperAuthenticator::authenticationCheckContent, [this](const QUrl &url, const QByteArray &content) {
        qDebug() << "current url: " << url;
        if(url.toString().startsWith("https://localhost:3001/simplesso/login")) {
            qDebug() << "incrementing i";
            m_ssoServerLoaded = true;
        }
        if(m_ssoServerLoaded && url == QUrl("https://localhost:3000/protectedContent")) {
            qDebug() << "returning true";
            return true;
        } else if(content == "<html><head></head><body><h1>protected content</h1></body></html>" && url == QUrl("https://localhost:3000/protectedContent")) {
            qDebug() << "returning true";
            return true;
        }
        return false;
    });

    authenticator->authenticate();
    loop.exec();
    m_ssoServerLoaded = false;
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
        return m_authenticator;
    }
    QRestWrapperNetworkHandler const * getNetworkHandler() const {
        return m_networkHandler;
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
    QEventLoop loop;
    qDebug() << __FUNCTION__;
    //m_restWrapperInstance->disconnect();
    connect(m_restWrapperInstance, &QRestWrapper::certificateError, [](const QRestWrapperCertificateError &error) {
        // ignore all errors related to self signed certificates
        return true;
    });
    QUrl *finalUrl = new QUrl("https://localhost/");
    TempWindow *newWindow = new TempWindow();
    connect(newWindow, &TempWindow::closed, [&loop]() {
        qDebug() << "closed.";
        //loop.exit(0);
        loop.quit();
    });
    connect(m_restWrapperInstance, &QRestWrapper::authenticated, [&finalUrl, &newWindow](const QUrl &url) {
        qDebug() << "authenticated.";
        QCOMPARE(url, QUrl("https://localhost:3000/protectedContent"));
        *finalUrl = url;
        //loop.quit();
        newWindow->close();
    });
    connect(m_restWrapperInstance, &QRestWrapper::authenticationCheckContent, [this](const QUrl &url, const QByteArray &content) {
        qDebug() << "current url: " << url;
        if(url.toString().startsWith("https://localhost:3001/simplesso/login")) {
            qDebug() << "incrementing i";
            m_ssoServerLoaded = true;
        }
        //qDebug() << "content: " << content;
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
        newWindow->setCentralWidget(widget);
        newWindow->show();
    });
    m_restWrapperInstance->authenticate();
    loop.exec();
    qDebug() << __FUNCTION__;
    QCOMPARE(*finalUrl, QUrl("https://localhost:3000/protectedContent"));
}

/*void QRestWrapperTest::authenticateCustomWithoutWindowTest()
{
    QEventLoop loop;
    qDebug() << __FUNCTION__;
    m_restWrapperInstance->disconnect();
    connect(m_restWrapperInstance, &QRestWrapper::certificateError, [](const QRestWrapperCertificateError &error) {
        // ignore all errors related to self signed certificates
        return true;
    });
    QUrl *finalUrl = new QUrl("https://localhost/");
    connect(m_restWrapperInstance, &QRestWrapper::closed, [&loop]() {
        qDebug() << "closed in QRestWrapperTest";
        loop.quit();
    });
    connect(m_restWrapperInstance, &QRestWrapper::authenticated, [&finalUrl, this](const QUrl &url) {
        qDebug() << "authenticated.";
        QCOMPARE(url, QUrl("https://localhost:3000/protectedContent"));
        *finalUrl = url;
        m_viewWidget->close();
    });
    connect(m_restWrapperInstance, &QRestWrapper::authenticationCheckContent, [this](const QUrl &url, const QByteArray &content) {
        qDebug() << "current url: " << url;
        if(url.toString().startsWith("https://localhost:3001/simplesso/login")) {
            qDebug() << "incrementing i";
            m_ssoServerLoaded = true;
        }
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
    });
    m_restWrapperInstance->authenticate();
    //QSignalSpy spy(m_restWrapperInstance, &QRestWrapper::closed);
    //while(spy.count() == 0) {
    //    QTest::qWait(200);
    //}
    loop.exec();
    //qDebug() << __FUNCTION__;
    //QCOMPARE(*finalUrl, QUrl("https://localhost:3000/protectedContent"));
}*/

/*void QRestWrapperTest::authenticateImplementedTest()
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
}*/

void QRestWrapperTest::cleanupTestCase()
{
    m_restWrapperInstance->deleteLater();
    m_restWrapperInstance = nullptr;
    QVERIFY(m_restWrapperInstance == nullptr);
}

//QTEST_MAIN(QRestWrapperTest)

//#include "tst_QRestWrapper.moc"
