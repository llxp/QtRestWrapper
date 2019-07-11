#ifndef QTRESTWRAPPERCAPI_H
#define QTRESTWRAPPERCAPI_H

#include "qtrestwrappercapi_global.h"

QTRESTWRAPPERCAPISHARED_EXPORT int64_t newQtRestWrapperInstance();
QTRESTWRAPPERCAPISHARED_EXPORT void deleteQtRestWrapperInstance(int64_t index);
QTRESTWRAPPERCAPISHARED_EXPORT void setStoragePath(int64_t index, const char *path, int length);
QTRESTWRAPPERCAPISHARED_EXPORT void setAuthenticationTestUrl(int64_t index, const char *url, int length);
QTRESTWRAPPERCAPISHARED_EXPORT void setApplicationUrl(int64_t index, const char *url, int length);
QTRESTWRAPPERCAPISHARED_EXPORT void authenticate(int64_t index);

QTRESTWRAPPERCAPISHARED_EXPORT void registerAddWebView(int64_t index, void (*ptr)(void *));



#endif // QTRESTWRAPPERCAPI_H
