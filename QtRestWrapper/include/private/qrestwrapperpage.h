#ifndef QRESTWRAPPERPAGE_H
#define QRESTWRAPPERPAGE_H

#include "../qrestwrappercertificateerror.h"

#include <QWebEnginePage>
#include <QWebEngineUrlRequestInterceptor>

#include "../qtrestwrapper_global.h"

class QTRESTWRAPPERSHARED_EXPORT QRestWrapperPage : public QWebEnginePage
{
    Q_OBJECT
public:
    explicit QRestWrapperPage(QObject *parent);
    virtual ~QRestWrapperPage() override;

public slots:
    void setRequestInterceptor(QWebEngineUrlRequestInterceptor *requestInterceptor);
    void setCachePath(const QString &path);
    void setPersistentStoragePath(const QString &path);

    // QWebEnginePage interface
private:
    virtual QStringList chooseFiles(FileSelectionMode mode, const QStringList &oldFiles, const QStringList &acceptedMimeTypes) override;
    virtual void javaScriptAlert(const QUrl &securityOrigin, const QString &msg) override;
    virtual bool javaScriptConfirm(const QUrl &securityOrigin, const QString &msg) override;
    virtual bool javaScriptPrompt(const QUrl &securityOrigin, const QString &msg, const QString &defaultValue, QString *result) override;
    virtual void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString &message, int lineNumber, const QString &sourceID) override;
    virtual bool certificateError(const QWebEngineCertificateError &certificateError) override;

signals:
    // will be called, if there is a ssl certificate error
    // return true to ignore the certificate error, return false to cancel the connection
    bool certificateErrorSignal(const QRestWrapperCertificateError &error);
};

#endif // QRESTWRAPPERPAGE_H
