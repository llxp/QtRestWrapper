#ifndef QTRESTWRAPPERCAPI_H
#define QTRESTWRAPPERCAPI_H

#include "qtrestwrappercapi_global.h"

#include "c_structures/qurl_c.h"
#include "c_structures/qrestwrappercertificateerror_c.h"

QTRESTWRAPPERCAPISHARED_EXPORT int64_t newQtRestWrapperInstance();
QTRESTWRAPPERCAPISHARED_EXPORT void deleteQtRestWrapperInstance(int64_t index);
QTRESTWRAPPERCAPISHARED_EXPORT void setStoragePath(int64_t index, const char *path, int length);
QTRESTWRAPPERCAPISHARED_EXPORT void setAuthenticationTestUrl(int64_t index, const char *url, int length);
QTRESTWRAPPERCAPISHARED_EXPORT void setApplicationUrl(int64_t index, const char *url, int length);
QTRESTWRAPPERCAPISHARED_EXPORT void authenticate(int64_t index);
QTRESTWRAPPERCAPISHARED_EXPORT void runJavaScript(int64_t index, const char *scriptSource, int length);
QTRESTWRAPPERCAPISHARED_EXPORT void runJavaScriptSynchronous(int64_t index, const char *scriptSource, int length);

QTRESTWRAPPERCAPISHARED_EXPORT void registerAddWebView(int64_t index, void (*ptr)(void *));
QTRESTWRAPPERCAPISHARED_EXPORT void registerClosed(int64_t index, void (*ptr)());
QTRESTWRAPPERCAPISHARED_EXPORT void registerAuthenticated(int64_t index, void (*ptr)(struct qurlCAPI *));
QTRESTWRAPPERCAPISHARED_EXPORT void registerAuthenticatedContent(int64_t index, void (*ptr)(struct qurlCAPI *, const char *));
QTRESTWRAPPERCAPISHARED_EXPORT void registerAuthenticationCheck(int64_t index, bool (*ptr)(struct qurlCAPI *));
QTRESTWRAPPERCAPISHARED_EXPORT void registerAuthenticationCheckContent(int64_t index, bool (*ptr)(struct qurlCAPI *, const char *));


#endif // QTRESTWRAPPERCAPI_H
