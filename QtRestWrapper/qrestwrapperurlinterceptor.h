#ifndef QRESTWRAPPERURLINTERCEPTOR_H
#define QRESTWRAPPERURLINTERCEPTOR_H

#include <QWebEngineUrlRequestInterceptor>

#include "qtrestwrapper_global.h"

namespace QtRestWrapper {

class QRestWrapperAuthenticator;
class QRestWrapperUrlInterceptorTest;

class QTRESTWRAPPERSHARED_EXPORT QRestWrapperUrlInterceptor : public QWebEngineUrlRequestInterceptor
{
    Q_OBJECT
    Q_DISABLE_COPY(QRestWrapperUrlInterceptor)
public:
    QRestWrapperUrlInterceptor(QRestWrapperAuthenticator *restWrapper, QObject *parent = nullptr);
    void setRestWrapperAuthenticator(QRestWrapperAuthenticator *restWrapper);
    void interceptRequest(QWebEngineUrlRequestInfo &info);

signals:
    void checkAuthenticationPage(const QUrl &url);

private:
    bool checkForAllowedList(const QVector<QString> &allowedList, const QUrl &url) const;
    bool checkForForbiddenList(const QVector<QString> &forbiddenList, const QUrl &url) const;

private:
    QRestWrapperAuthenticator *m_restWrapperAuthenticator;

private:
    friend class QRestWrapperUrlInterceptorTest;
};

} // namespace QtRestWrapper

#endif // QRESTWRAPPERURLINTERCEPTOR_H
