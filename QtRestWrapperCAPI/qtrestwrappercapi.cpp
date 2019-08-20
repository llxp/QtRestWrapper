#include "qtrestwrappercapi.h"

#include <vector>
#include <map>

#include <qrestwrapper.h>

#include <QMutex>
#include <QWidget>

#include <c_structures/qurl_c.h>
#include <c_structures/qrestwrappercertificateerror_c.h>
#include <c_structures/qvariant_capi.h>

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
void callFunction(int64_t index, T function, Rest... rest) {
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

const char *runJavaScriptSynchronous(int64_t index, const char *scriptSource, int length)
{
    QVariant *retPtr = new QVariant(callFunction2(index, &QtRestWrapper::QRestWrapper::runJavaScriptSynchronous, QString::fromUtf8(scriptSource, length)));
    /*struct  qvariant_capi newObj;
    QVariant::Private privateDataPtr = retPtr->data_ptr();
    union {
        QVariant::Private::Data data;
        qvariant_capi::Data data_capi;
    } cast_helper;
    cast_helper.data = privateDataPtr.data;
    newObj.data = cast_helper.data_capi;
    newObj.type = privateDataPtr.type;
    newObj.is_null = privateDataPtr.is_null;
    newObj.is_shared = privateDataPtr.is_shared;*/
    QByteArray strData = retPtr->toString().toUtf8();
    char *newStr = static_cast<char *>(malloc(sizeof(char) * static_cast<size_t>(strData.length()) + 1));
    strcpy_s(newStr, static_cast<size_t>(strData.length()), strData.data());
    newStr[strData.length()] = '\0';
    return newStr;
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

void registerAuthenticated(int64_t index, void (*ptr)(struct qurlCAPI))
{
    using QtRestWrapper::QRestWrapper;
    registerPtr(index, &QRestWrapper::authenticated, [ptr](const QUrl &url) {
        QByteArray urlData = url.toString().toUtf8();
        ptr(qurlCapiFromString(urlData.data()));
    });
}

void registerAuthenticatedContent(int64_t index, void (*ptr)(struct qurlCAPI, const char *))
{
    using QtRestWrapper::QRestWrapper;
    registerPtr(index, &QRestWrapper::authenticatedContent, [ptr](const QUrl &url, const QByteArray &content) {
        QByteArray urlData = url.toString().toUtf8();
        ptr(qurlCapiFromString(urlData.data()), content.data());
    });
}

void registerAuthenticationCheck(int64_t index, bool (*ptr)(struct qurlCAPI))
{
    using QtRestWrapper::QRestWrapper;
    registerPtr(index, &QRestWrapper::authenticationCheck, [ptr](const QUrl &url) -> bool {
        QByteArray urlData = url.toString().toUtf8();
        return ptr(qurlCapiFromString(urlData.data()));
    });
}

void registerAuthenticationCheckContent(int64_t index, bool (*ptr)(struct qurlCAPI, const char *))
{
    using QtRestWrapper::QRestWrapper;
    registerPtr(index, &QRestWrapper::authenticationCheckContent, [ptr](const QUrl &url, const QByteArray &content) -> bool {
        QByteArray urlData = url.toString().toUtf8();
        return ptr(qurlCapiFromString(urlData.data()), content.data());
    });
}

void registerCertificateError(int64_t index, bool (*ptr)(struct QRestWrapperCertificateError_CAPI))
{
    using QtRestWrapper::QRestWrapper;
    registerPtr(index, &QRestWrapper::certificateError, [ptr](const QRestWrapperCertificateError &error) -> bool {
        return ptr(qRestWrapperCertificateErrorCapiFromQRestWrapperCertificateError(error));
    });
}

void registerScriptEnded(int64_t index, void (*ptr)(const char *))
{
    using QtRestWrapper::QRestWrapper;
    registerPtr(index, &QRestWrapper::scriptEnded, [ptr](const QVariant &result) {
        QByteArray strData = result.toString().toUtf8();
        char *newStr = static_cast<char *>(malloc(sizeof(char) * static_cast<size_t>(strData.length()) + 1));
        strcpy_s(newStr, static_cast<size_t>(strData.length()), strData.data());
        newStr[strData.length()] = '\0';
        ptr(newStr);
    });
}

void registerContentReady(int64_t index, void (*ptr)(const qurlCAPI, const char *))
{
    using QtRestWrapper::QRestWrapper;
    registerPtr(index, &QRestWrapper::contentReady, [ptr](const QUrl &url, const QByteArray &content) {
        QByteArray urlData = url.toString().toUtf8();
        ptr(qurlCapiFromString(urlData.data()), content.data());
    });
}

void registerAuthenticationFailure(int64_t index, void (*ptr)(const qurlCAPI, const char *, int))
{
    using QtRestWrapper::QRestWrapper;
    registerPtr(index, &QRestWrapper::authenticationFailure, [ptr](const QUrl &url, const QByteArray &content, int statusCode) {
        QByteArray urlData = url.toString().toUtf8();
        ptr(qurlCapiFromString(urlData.data()), content.data(), statusCode);
    });
}

void registerNotFound(int64_t index, void (*ptr)(struct qurlCAPI, const char *))
{
    using QtRestWrapper::QRestWrapper;
    registerPtr(index, &QRestWrapper::notFound, [ptr](const QUrl &url, const QByteArray &content) {
        QByteArray urlData = url.toString().toUtf8();
        return ptr(qurlCapiFromString(urlData.data()), content.data());
    });
}

const QMap<QString, QString> toHeaderNodes(struct requestHeader headers) {
    QMap<QString, QString> headerNodes;
    for (unsigned long long i = 0; i < headers.nodesCount;++i) {
        struct requestHeaderNode currentNode = headers.nodes[i];
        headerNodes[QString::fromUtf8(currentNode.name)] = QString::fromUtf8(currentNode.value);
    }
    return headerNodes;
}

void sendGetRequest(int64_t index, struct qurlCAPI url, struct requestHeader headers)
{
    callFunction(index, &QtRestWrapper::QRestWrapper::sendGetRequest, *static_cast<QUrl *>(url.originalQUrl), toHeaderNodes(headers));
}

void sendPutRequest(int64_t index, struct qurlCAPI url, const char *payload, const char *contentType, requestHeader headers)
{
    QByteArray payloadData = QString::fromUtf8(payload).toUtf8();
    QString contentTypeData = QString::fromUtf8(contentType);
    QUrl urlData = *static_cast<QUrl *>(url.originalQUrl);
    typedef void (QtRestWrapper::QRestWrapper::*funcType)(const QUrl &,
                                                          const QByteArray &,
                                                          const QString &,
                                                          const QMap<QString, QString> &);
    callFunction
            <funcType, QUrl, QByteArray, QString, QMap<QString, QString>>
            (index,
             &QtRestWrapper::QRestWrapper::sendPutRequest,
             urlData,
             payloadData,
             contentTypeData,
             toHeaderNodes(headers));
}

void sendDeleteRequest(int64_t index, qurlCAPI url, const char *payload, const char *contentType, requestHeader headers)
{
    QByteArray payloadData = QString::fromUtf8(payload).toUtf8();
    QString contentTypeData = QString::fromUtf8(contentType);
    QUrl urlData = *static_cast<QUrl *>(url.originalQUrl);
    typedef void (QtRestWrapper::QRestWrapper::*funcType)(const QUrl &,
                                                          const QByteArray &,
                                                          const QString &,
                                                          const QMap<QString, QString> &);
    callFunction
            <funcType, QUrl, QByteArray, QString, QMap<QString, QString>>
            (index,
             &QtRestWrapper::QRestWrapper::sendDeleteRequest,
             urlData,
             payloadData,
             contentTypeData,
             toHeaderNodes(headers));
}

void sendOptionsRequest(int64_t index, qurlCAPI url, requestHeader headers)
{
    QUrl urlData = *static_cast<QUrl *>(url.originalQUrl);
    callFunction(index, &QtRestWrapper::QRestWrapper::sendOptionsRequest, urlData, toHeaderNodes(headers));
}

void sendPostRequest(int64_t index, qurlCAPI url, const char *payload, const char *contentType, requestHeader headers)
{
    QByteArray payloadData = QString::fromUtf8(payload).toUtf8();
    QString contentTypeData = QString::fromUtf8(contentType);
    QUrl urlData = *static_cast<QUrl *>(url.originalQUrl);
    typedef void (QtRestWrapper::QRestWrapper::*funcType)(const QUrl &,
                                                          const QByteArray &,
                                                          const QString &,
                                                          const QMap<QString, QString> &);
    callFunction
            <funcType, QUrl, QByteArray, QString, QMap<QString, QString>>
            (index,
             &QtRestWrapper::QRestWrapper::sendPostRequest,
             urlData,
             payloadData,
             contentTypeData,
             toHeaderNodes(headers));
}

void sendRequest(int64_t index, qurlCAPI url, const char *verb, const char *payload, const char *contentType, requestHeader headers)
{
    QByteArray payloadData = QString::fromUtf8(payload).toUtf8();
    QSharedPointer<QByteArray> payloadDataPointer = QSharedPointer<QByteArray>::create(payloadData);
    QString contentTypeData = QString::fromUtf8(contentType);
    QUrl urlData = *static_cast<QUrl *>(url.originalQUrl);
    QString verbData = QString::fromUtf8(verb);
    typedef void (QtRestWrapper::QRestWrapper::*funcType)(const QUrl &,
                                                          const QSharedPointer<QByteArray> &,
                                                          const QString &,
                                                          const QString &,
                                                          const QMap<QString, QString> &);
    callFunction
            <funcType, QUrl, QSharedPointer<QByteArray>, QString, QString, QMap<QString, QString>>
            (index,
             &QtRestWrapper::QRestWrapper::sendRequest,
             urlData,
             payloadDataPointer,
             verbData,
             contentTypeData,
             toHeaderNodes(headers));
}

void deleteCookiesByName(int64_t index, const char *cookieName)
{
    callFunction(index, &QtRestWrapper::QRestWrapper::deleteCookiesByName, QString::fromUtf8(cookieName));
}

void deleteCookiesByDomain(int64_t index, const char *cookieDomain)
{
    callFunction(index, &QtRestWrapper::QRestWrapper::deleteCookiesByDomain, QString::fromUtf8(cookieDomain));
}

void deleteCookiesByPath(int64_t index, const char *cookiePath)
{
    callFunction(index, &QtRestWrapper::QRestWrapper::deleteCookiesByPath, QString::fromUtf8(cookiePath));
}

void deleteCookiesByNameAndDomain(int64_t index, const char *cookieName, const char *cookieDomain)
{
    callFunction(index, &QtRestWrapper::QRestWrapper::deleteCookiesByNameAndDomain, QString::fromUtf8(cookieName), QString::fromUtf8(cookieDomain));
}

void deleteCookiesByNameAndDomainAndPath(int64_t index, const char *cookieName, const char *cookieDomain, const char *cookiePath)
{
    callFunction(index, &QtRestWrapper::QRestWrapper::deleteCookiesByNameAndDomainAndPath, QString::fromUtf8(cookieName), QString::fromUtf8(cookieDomain), QString::fromUtf8(cookiePath));
}

void clearAllCookies(int64_t index)
{
    callFunction(index, &QtRestWrapper::QRestWrapper::clearAllCookies);
}

const char **getOriginalCookieStringsByName(int64_t index, const char *cookieName)
{
    QVector<QString> ret = callFunction2(index, &QtRestWrapper::QRestWrapper::getOriginalCookieStringsByName, QString::fromUtf8(cookieName));
    std::vector<const char *> *ptrs = new std::vector<const char *>();
    for(auto str: ret) {
        char *buffer = static_cast<char *>(malloc(sizeof(char) * static_cast<size_t>(str.length()) + 1));
        strcpy_s(buffer, static_cast<size_t>(str.length()), str.toUtf8().data());
        buffer[str.length()] = '\0';
        ptrs->push_back(buffer);
    }
    return ptrs->data();
}
