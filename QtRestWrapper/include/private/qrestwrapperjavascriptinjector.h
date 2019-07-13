#ifndef QRESTWRAPPERJAVASCRIPTINJECTOR_H
#define QRESTWRAPPERJAVASCRIPTINJECTOR_H

#include <QEventLoop>
#include <QObject>
#include <QWebEnginePage>

class QRestWrapperJavascriptInjector : public QObject
{
    Q_OBJECT
public:
    explicit QRestWrapperJavascriptInjector(QWebEnginePage *page, QObject *parent = nullptr);
    void runJavaScript(const QString &scriptSource);
    QVariant runJavaScriptSynchronous(const QString &scriptSource);

signals:
    void scriptEnded(const QVariant &result);

private slots:
    void runJavascriptPrivate(const QString &scriptSource);

protected:
    QWebEnginePage *m_page;
    QVariant m_scriptResult;
    QEventLoop *m_eventLoop;
};

#endif // QRESTWRAPPERJAVASCRIPTINJECTOR_H
