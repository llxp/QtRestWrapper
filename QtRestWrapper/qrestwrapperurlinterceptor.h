#ifndef QRESTWRAPPERURLINTERCEPTOR_H
#define QRESTWRAPPERURLINTERCEPTOR_H

#include <QWebEngineUrlRequestInterceptor>



namespace QtRestWrapper {

class QRestWrapper;

class QRestWrapperUrlInterceptor : public QWebEngineUrlRequestInterceptor
{
    Q_OBJECT
    Q_DISABLE_COPY(QRestWrapperUrlInterceptor)
public:
    QRestWrapperUrlInterceptor(QObject *parent = nullptr);
    void setRestWrapper(QRestWrapper *restWrapper);
    void interceptRequest(QWebEngineUrlRequestInfo &info);

signals:
    void checkAuthenticationPage(const QUrl &url);

private:
    QRestWrapper *m_restWrapper;
};

} // namespace QtRestWrapper

#endif // QRESTWRAPPERURLINTERCEPTOR_H
