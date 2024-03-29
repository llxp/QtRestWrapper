#include "../../include/private/qrestwrapperpage.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QWebEngineCertificateError>
#include <QWebEngineProfile>
#include <QWebEngineUrlRequestInterceptor>
#include <QWebEngineSettings>
#include <QEventLoop>

QRestWrapperPage::QRestWrapperPage(QObject *parent)
    : QWebEnginePage(new QWebEngineProfile(parent), /*create a dummy object*/new QObject(parent))
{
    //profile()->setParent(parent);
    profile()->setPersistentCookiesPolicy(QWebEngineProfile::AllowPersistentCookies);
    //settings()->setAttribute(QWebEngineSettings::AutoLoadImages, false);
    //settings()->setAttribute(QWebEngineSettings::AutoLoadIconsForPage, false);
    QObject::connect(this, &QWebEnginePage::certificateError, this, &QRestWrapperPage::certificateError);
}

QRestWrapperPage::~QRestWrapperPage()
{
    disconnect();
    qDebug() << __FUNCTION__;
    //profile()->setParent(nullptr);
    profile()->disconnect();
    profile()->setUrlRequestInterceptor(nullptr);
    //delete profile();
    //profile()->deleteLater();
}

void QRestWrapperPage::setRequestInterceptor(QWebEngineUrlRequestInterceptor *requestInterceptor)
{
    profile()->setUrlRequestInterceptor(requestInterceptor);
}

void QRestWrapperPage::setCachePath(const QString &path)
{
    profile()->setCachePath(path);
}

void QRestWrapperPage::setPersistentStoragePath(const QString &path)
{
    profile()->setPersistentStoragePath(path);
}


QStringList QRestWrapperPage::chooseFiles(QWebEnginePage::FileSelectionMode mode, const QStringList &oldFiles, const QStringList &acceptedMimeTypes)
{
    qDebug() << __FUNCTION__;
    QFileDialog openFileDialog(qobject_cast<QWidget *>(this->parent()));
    openFileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    openFileDialog.setMimeTypeFilters(acceptedMimeTypes);
    openFileDialog.exec();
    return openFileDialog.selectedFiles();
}

void QRestWrapperPage::javaScriptAlert(const QUrl &securityOrigin, const QString &msg)
{
    qDebug() << __FUNCTION__;
    QMessageBox::information(qobject_cast<QWidget *>(this->parent()), securityOrigin.host() + " says: ", msg, QMessageBox::Ok);
}

bool QRestWrapperPage::javaScriptConfirm(const QUrl &securityOrigin, const QString &msg)
{
    qDebug() << __FUNCTION__;
    QMessageBox::StandardButton result = QMessageBox::question(qobject_cast<QWidget *>(this->parent()), securityOrigin.host() + " says: ", msg, QMessageBox::Yes | QMessageBox::No);
    if(result == QMessageBox::Yes) {
        return true;
    } else {
        return false;
    }
}

bool QRestWrapperPage::javaScriptPrompt(const QUrl &securityOrigin, const QString &msg, const QString &defaultValue, QString *result)
{
    qDebug() << __FUNCTION__;
    bool ok;
    QString text = QInputDialog::getText(qobject_cast<QWidget *>(this->parent()),
                                         securityOrigin.host() + " says: ",
                                         msg,
                                         QLineEdit::Normal,
                                         defaultValue, &ok);
    if (ok) {
        result = new QString(text);
        return true;
    }
    return false;
}

void QRestWrapperPage::javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, const QString &message, int lineNumber, const QString &sourceID)
{
    qDebug() << __FUNCTION__;
    switch(level) {
    case QWebEnginePage::JavaScriptConsoleMessageLevel::InfoMessageLevel:
        qDebug() << "INFO: " << message << "\n" << "on line: " << lineNumber << " in " << sourceID;
        break;
    case QWebEnginePage::JavaScriptConsoleMessageLevel::ErrorMessageLevel:
        qCritical() << "ERROR: " << message << "\n" << "on line: " << lineNumber << " in " << sourceID;
        break;
    case QWebEnginePage::JavaScriptConsoleMessageLevel::WarningMessageLevel:
        qWarning() << "WARNING: " << message << "\n" << "on line: " << lineNumber << " in " << sourceID;
        break;
    }
}

void QRestWrapperPage::certificateErrorHandler(const QWebEngineCertificateError &certificateError)
{
    qDebug() << __FUNCTION__;
    QRestWrapperCertificateError::Error newError = static_cast<QRestWrapperCertificateError::Error>(certificateError.type());
    QRestWrapperCertificateError error(newError, certificateError.description(), certificateError.isOverridable(), certificateError.url());
    auto mutableError = const_cast<QWebEngineCertificateError&>(certificateError);
    if (emit certificateErrorSignal(error)) {
        mutableError.acceptCertificate();
    } else {
        mutableError.rejectCertificate();
    }
}
