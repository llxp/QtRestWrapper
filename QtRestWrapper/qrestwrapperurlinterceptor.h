#ifndef QRESTWRAPPERURLINTERCEPTOR_H
#define QRESTWRAPPERURLINTERCEPTOR_H

#include <QWebEngineUrlRequestInterceptor>

#include "qtrestwrapper_global.h"

namespace QtRestWrapper {

class QRestWrapperPrivate;
class QRestWrapperUrlInterceptorTest;

class QTRESTWRAPPERSHARED_EXPORT QRestWrapperUrlInterceptor : public QWebEngineUrlRequestInterceptor
{
    Q_OBJECT
    Q_DISABLE_COPY(QRestWrapperUrlInterceptor)
public:
    QRestWrapperUrlInterceptor(QObject *parent = nullptr);
    void setRestWrapper(QRestWrapperPrivate *restWrapper);
    void interceptRequest(QWebEngineUrlRequestInfo &info);

signals:
    void checkAuthenticationPage(const QUrl &url);

private:
    bool checkForAllowedList(const QVector<QString> &allowedList, const QUrl &url) const;
    bool checkForForbiddenList(const QVector<QString> &forbiddenList, const QUrl &url) const;

private:
    QRestWrapperPrivate *m_restWrapper;

private:
    friend class QRestWrapperUrlInterceptorTest;
};

} // namespace QtRestWrapper

#endif // QRESTWRAPPERURLINTERCEPTOR_H
