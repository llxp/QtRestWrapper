#include "qtrestwrappercapi.h"

#include <vector>
#include <map>

#include <qrestwrapper.h>

#include <QMutex>
#include <QWidget>

#include <c_structures/qurl_c.h>
#include <c_structures/qrestwrappercertificateerror_c.h>

static std::map<int64_t, std::pair<QtRestWrapper::QRestWrapper *, QMutex *>> restWrapperInstances;

static QMutex mCreateInstances;

int64_t newQtRestWrapperInstance()
{
    using QtRestWrapper::QRestWrapper;
    QRestWrapper *newInstance = new QRestWrapper(nullptr);
    if(newInstance != nullptr) {
        QMutexLocker lock(&mCreateInstances);
        restWrapperInstances[reinterpret_cast<int64_t>(newInstance)] = std::pair<QtRestWrapper::QRestWrapper *, QMutex *>(newInstance, new QMutex());
        lock.unlock();
        return reinterpret_cast<int64_t>(newInstance);
    }
    return 0;
}

QtRestWrapper::QRestWrapper *getInstance(int64_t index) {
    return restWrapperInstances[index].first;
}

QMutex *getMutex(int64_t index) {
    return restWrapperInstances[index].second;
}

void deleteQtRestWrapperInstance(int64_t index)
{
    QMutexLocker lock(&mCreateInstances);
    using QtRestWrapper::QRestWrapper;
    QMutexLocker lock2(getMutex(index));
    QRestWrapper *newInstance = getInstance(index);
    newInstance->deleteLater();
    restWrapperInstances.erase(index);
    lock2.unlock();
    delete getMutex(index);
    lock.unlock();
}

#define executeFunction(index, function, ...) \
    QMutexLocker lock(&mCreateInstances); \
    using QtRestWrapper::QRestWrapper; \
    QRestWrapper *newInstance = getInstance(index); \
    QMutexLocker lock2(getMutex(index)); \
    newInstance->function(__VA_ARGS__); \
    lock2.unlock(); \
    lock.unlock();

#define executeFunction2(index, function, ...) \
    QMutexLocker lock(&mCreateInstances); \
    using QtRestWrapper::QRestWrapper; \
    QRestWrapper *newInstance = getInstance(index); \
    QMutexLocker lock2(getMutex(index)); \
    auto returnValue = newInstance->function(__VA_ARGS__); \
    lock2.unlock(); \
    lock.unlock(); \
    return returnValue;

template<typename T, typename ...Rest>
auto callFunction(int64_t index, T function, Rest... rest) {
    QMutexLocker lock(&mCreateInstances);
    using QtRestWrapper::QRestWrapper;
    QRestWrapper *newInstance = getInstance(index);
    QMutexLocker lock2(getMutex(index));
    (*newInstance.*function)(rest...);
    lock2.unlock();
    lock.unlock();
}

template<typename T, typename ...Rest>
auto callFunction2(int64_t index, T function, Rest... rest) {
    QMutexLocker lock(&mCreateInstances);
    using QtRestWrapper::QRestWrapper;
    QRestWrapper *newInstance = getInstance(index);
    QMutexLocker lock2(getMutex(index));
    auto returnValue = (*newInstance.*function)(rest...);
    lock2.unlock();
    lock.unlock();
    return returnValue;
}

void setStoragePath(int64_t index, const char *path, int length)
{
    callFunction(index, &QtRestWrapper::QRestWrapper::setStoragePath, QString::fromUtf8(path, length));
    //executeFunction(index, setStoragePath, QString::fromUtf8(path, length));
}

void setAuthenticationTestUrl(int64_t index, const char *url, int length)
{
    callFunction(index, &QtRestWrapper::QRestWrapper::setAuthenticationTestUrl, QUrl(QString::fromUtf8(url, length)));
    //executeFunction(index, setAuthenticationTestUrl, QUrl(QString::fromUtf8(url, length)));
}

void setApplicationUrl(int64_t index, const char *url, int length)
{
    callFunction(index, &QtRestWrapper::QRestWrapper::setApplicationUrl, QUrl(QString::fromUtf8(url, length)));
    //executeFunction(index, setApplicationUrl, QUrl(QString::fromUtf8(url, length)));
}

void authenticate(int64_t index)
{
    callFunction(index, &QtRestWrapper::QRestWrapper::authenticate);
    //executeFunction(index, authenticate);
}

void runJavaScript(int64_t index, const char *scriptSource, int length)
{
    callFunction(index, &QtRestWrapper::QRestWrapper::runJavaScript, QString::fromUtf8(scriptSource, length));
}

void runJavaScriptSynchronous(int64_t index, const char *scriptSource, int length)
{
    return callFunction(index, &QtRestWrapper::QRestWrapper::runJavaScriptSynchronous, QString::fromUtf8(scriptSource, length));
}

template <typename Func1, typename Func2>
void registerPtr(int64_t index, Func1 signal, Func2 ptr) {
    QMutexLocker lock(&mCreateInstances);
    using QtRestWrapper::QRestWrapper;
    QRestWrapper *newInstance = getInstance(index);
    QMutexLocker lock2(getMutex(index));
    newInstance->connect(newInstance, signal, ptr);
    lock2.unlock();
    lock.unlock();
}

void registerAddWebView(int64_t index, void (*ptr)(void *))
{
    using QtRestWrapper::QRestWrapper;
    registerPtr(index, &QRestWrapper::addWebView, ptr);
}

void registerClosed(int64_t index, void (*ptr)())
{
    using QtRestWrapper::QRestWrapper;
    registerPtr(index, &QRestWrapper::closed, ptr);
}

void registerAuthenticated(int64_t index, void (*ptr)(struct qurlCAPI *))
{
    using QtRestWrapper::QRestWrapper;
    registerPtr(index, &QRestWrapper::authenticated, [ptr](const QUrl &url) {
        ptr(qurlCapiFromString(nullptr, url.toString().toUtf8().data()));
    });
}

void registerAuthenticatedContent(int64_t index, void (*ptr)(struct qurlCAPI *, const char *))
{
    using QtRestWrapper::QRestWrapper;
    registerPtr(index, &QRestWrapper::authenticatedContent, [ptr](const QUrl &url, const QByteArray &content) {
        ptr(qurlCapiFromString(nullptr, url.toString().toUtf8().data()), content.data());
    });
}

void registerAuthenticationCheck(int64_t index, bool (*ptr)(struct qurlCAPI *))
{
    using QtRestWrapper::QRestWrapper;
    registerPtr(index, &QRestWrapper::authenticationCheck, [ptr](const QUrl &url) -> bool {
        return ptr(qurlCapiFromString(nullptr, url.toString().toUtf8().data()));
    });
}

void registerAuthenticationCheckContent(int64_t index, bool (*ptr)(struct qurlCAPI *, const char *))
{
    using QtRestWrapper::QRestWrapper;
    registerPtr(index, &QRestWrapper::authenticationCheckContent, [ptr](const QUrl &url, const QByteArray &content) -> bool {
        return ptr(qurlCapiFromString(nullptr, url.toString().toUtf8().data()), content.data());
    });
}

void registerCertificateError(int64_t index, bool (*ptr)(struct QRestWrapperCertificateError_CAPI *))
{
    using QtRestWrapper::QRestWrapper;
    registerPtr(index, &QRestWrapper::certificateError, [ptr](const QRestWrapperCertificateError &error) -> bool {
        return ptr(qRestWrapperCertificateErrorCapiFromQRestWrapperCertificateError(error));
    });
}
