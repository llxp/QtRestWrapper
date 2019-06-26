#include "qrestwrapperjavascriptinjector.h"

#include <QEventLoop>
#include <QTimer>

#include <type_traits>

class FunctorInjectorAdapter : public QRestWrapperJavascriptInjector {
public:
    void setJavascriptResult(const QVariant &result) {
        m_scriptResult = result;
        emit m_eventLoop->quit();
        m_eventLoop = nullptr;
    }
};

template<bool enable = false>
class Functor
{
public:
    Functor(QRestWrapperJavascriptInjector & javascriptInjector) :
        m_javascriptInjector(javascriptInjector) {}

public:
    void operator()(const QVariant & data)
    {
        operatorInternal(data);
    }

private:
    template<bool enabled = enable>
    inline void operatorInternal(typename std::enable_if<enabled, const QVariant &>::type data)
    {
        m_javascriptInjector.scriptEnded(data);
    }

    template<bool enabled = enable>
    inline void operatorInternal(typename std::enable_if<!enabled, const QVariant &>::type data)
    {
        FunctorInjectorAdapter * ptr = static_cast<FunctorInjectorAdapter *>(&m_javascriptInjector);
        ptr->setJavascriptResult(data);
    }

private:
    QRestWrapperJavascriptInjector &m_javascriptInjector;
    bool m_invokeSignal;
};

QRestWrapperJavascriptInjector::QRestWrapperJavascriptInjector(QWebEnginePage *page, QObject *parent) :
    QObject(parent),
    m_page(page)
{

}

void QRestWrapperJavascriptInjector::runJavaScript(const QString &scriptSource)
{
    m_page->runJavaScript(scriptSource, Functor<true>(*this));
}

QVariant QRestWrapperJavascriptInjector::runJavaScriptSynchronous(const QString &scriptSource)
{
    QEventLoop loop;
    m_eventLoop = &loop;

    QTimer::singleShot(0, this, [this, &scriptSource]() -> void {
        emit runJavascriptPrivate(scriptSource);
    });

    loop.exec();

    return m_scriptResult;
}

void QRestWrapperJavascriptInjector::runJavascriptPrivate(const QString &scriptSource)
{
    m_page->runJavaScript(scriptSource, Functor<false>(*this));
}
