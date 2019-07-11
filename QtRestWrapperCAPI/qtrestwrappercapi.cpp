#include "qtrestwrappercapi.h"

#include <vector>

#include "../QtRestWrapper/qrestwrapper.h"

#include <QMutex>

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

void deleteQtRestWrapperInstance(int64_t index)
{
    QMutexLocker lock(&mCreateInstances);
    using QtRestWrapper::QRestWrapper;
    QMutexLocker lock2(restWrapperInstances[index].second);
    QRestWrapper *newInstance = restWrapperInstances[index].first;
    newInstance->deleteLater();
    restWrapperInstances.erase(index);
    lock2.unlock();
    delete restWrapperInstances[index].second;
    lock.unlock();
}

#define executeFunction(index, function, ...) \
    QMutexLocker lock(&mCreateInstances); \
    using QtRestWrapper::QRestWrapper; \
    QRestWrapper *newInstance = restWrapperInstances[index].first; \
    QMutexLocker lock2(restWrapperInstances[index].second); \
    newInstance->function(__VA_ARGS__); \
    lock2.unlock(); \
    lock.unlock();

#define executeFunction2(index, function, ...) \
    QMutexLocker lock(&mCreateInstances); \
    using QtRestWrapper::QRestWrapper; \
    QRestWrapper *newInstance = restWrapperInstances[index].first; \
    QMutexLocker lock2(restWrapperInstances[index].second); \
    auto returnValue = newInstance->function(__VA_ARGS__); \
    lock2.unlock(); \
    lock.unlock(); \
    return returnValue;

void setStoragePath(int64_t index, const char *path, int length)
{
    executeFunction(index, setStoragePath, QString::fromUtf8(path, length));
}

void setAuthenticationTestUrl(int64_t index, const char *url, int length)
{
    executeFunction(index, setAuthenticationTestUrl, QUrl(QString::fromUtf8(url, length)));
}

void setApplicationUrl(int64_t index, const char *url, int length)
{
    executeFunction(index, setApplicationUrl, QUrl(QString::fromUtf8(url, length)));
}

void authenticate(int64_t index)
{
    executeFunction(index, authenticate);
}

void registerAddWebView(int64_t index, void (*ptr)(void *))
{
    QMutexLocker lock(&mCreateInstances);
    using QtRestWrapper::QRestWrapper;
    QRestWrapper *newInstance = restWrapperInstances[index].first;
    QMutexLocker lock2(restWrapperInstances[index].second);
    newInstance->connect(newInstance, &QRestWrapper::addWebView, [ptr](QWidget *widget) {
        ptr(widget);
    });
    lock2.unlock();
    lock.unlock();
}
