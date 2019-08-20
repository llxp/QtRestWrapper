#ifndef QTRESTWRAPPERCAPI_H
#define QTRESTWRAPPERCAPI_H

#include "qtrestwrappercapi_global.h"

#include <c_structures/qurl_c.h>
#include <c_structures/qrestwrappercertificateerror_c.h>
#include <c_structures/qvariant_capi.h>
#include <c_structures/qnetworkcookie_capi.h>

struct requestHeaderNode {
    char *name;
    char *value;
};

struct requestHeader {
    struct requestHeaderNode *nodes;
    unsigned long long nodesCount;
};

QTRESTWRAPPERCAPISHARED_EXPORT int64_t newQtRestWrapperInstance();
QTRESTWRAPPERCAPISHARED_EXPORT void deleteQtRestWrapperInstance(int64_t index);
QTRESTWRAPPERCAPISHARED_EXPORT void setStoragePath(int64_t index, const char *path, int length);
QTRESTWRAPPERCAPISHARED_EXPORT void setAuthenticationTestUrl(int64_t index, const char *url, int length);
QTRESTWRAPPERCAPISHARED_EXPORT void setApplicationUrl(int64_t index, const char *url, int length);
QTRESTWRAPPERCAPISHARED_EXPORT void authenticate(int64_t index);
QTRESTWRAPPERCAPISHARED_EXPORT void runJavaScript(int64_t index, const char *scriptSource, int length);
QTRESTWRAPPERCAPISHARED_EXPORT const char *runJavaScriptSynchronous(int64_t index, const char *scriptSource, int length);

QTRESTWRAPPERCAPISHARED_EXPORT void sendGetRequest(int64_t index, struct qurlCAPI url, struct requestHeader headers);
QTRESTWRAPPERCAPISHARED_EXPORT void sendPutRequest(int64_t index, struct qurlCAPI url, const char *payload, const char *contentType, struct requestHeader headers);
QTRESTWRAPPERCAPISHARED_EXPORT void sendDeleteRequest(int64_t index, struct qurlCAPI url, const char *payload, const char *contentType, struct requestHeader headers);
QTRESTWRAPPERCAPISHARED_EXPORT void sendOptionsRequest(int64_t index, struct qurlCAPI url, struct requestHeader headers);
QTRESTWRAPPERCAPISHARED_EXPORT void sendPostRequest(int64_t index, struct qurlCAPI url, const char *payload, const char *contentType, struct requestHeader headers);
QTRESTWRAPPERCAPISHARED_EXPORT void sendRequest(int64_t index, struct qurlCAPI url, const char *verb, const char *payload, const char *contentType, struct requestHeader headers);

QTRESTWRAPPERCAPISHARED_EXPORT void deleteCookiesByName(int64_t index, const char *cookieName);
QTRESTWRAPPERCAPISHARED_EXPORT void deleteCookiesByDomain(int64_t index, const char *cookieDomain);
QTRESTWRAPPERCAPISHARED_EXPORT void deleteCookiesByPath(int64_t index, const char *cookiePath);
QTRESTWRAPPERCAPISHARED_EXPORT void deleteCookiesByNameAndDomain(int64_t index, const char *cookieName, const char *cookieDomain);
QTRESTWRAPPERCAPISHARED_EXPORT void deleteCookiesByNameAndDomainAndPath(int64_t index, const char *cookieName, const char *cookieDomain, const char *cookiePath);
QTRESTWRAPPERCAPISHARED_EXPORT void clearAllCookies(int64_t index);

// methods to get cookies by identifiers
QTRESTWRAPPERCAPISHARED_EXPORT qnetworkcookieListCAPI getCookiesByName(int64_t index, const char *cookieName);
QTRESTWRAPPERCAPISHARED_EXPORT qnetworkcookieListCAPI getCookiesByDomain(int64_t index, const char *cookieDomain);
QTRESTWRAPPERCAPISHARED_EXPORT qnetworkcookieListCAPI getCookiesByPath(int64_t index, const char *cookiePath);
QTRESTWRAPPERCAPISHARED_EXPORT qnetworkcookieListCAPI getCookiesByNameAndDomain(int64_t index, const char *cookieName, const char *cookieDomain);
QTRESTWRAPPERCAPISHARED_EXPORT qnetworkcookieListCAPI getCookiesByNameAndDomainAndPath(int64_t index, const char *cookieName, const char *cookieDomain, const char *cookiePath);
QTRESTWRAPPERCAPISHARED_EXPORT qnetworkcookieListCAPI getAllCookies(int64_t index);

// methods to get the raw cookie strings by identifiers
QTRESTWRAPPERCAPISHARED_EXPORT const char **getOriginalCookieStringsByName(int64_t index, const char *cookieName);
QTRESTWRAPPERCAPISHARED_EXPORT const char **getOriginalCookieStringsByDomain(int64_t index, const char *cookieDomain);
QTRESTWRAPPERCAPISHARED_EXPORT const char **getOriginalCookieStringsByPath(int64_t index, const char *cookiePath);
QTRESTWRAPPERCAPISHARED_EXPORT const char **getOriginalCookieStringsByNameAndDomain(int64_t index, const char *cookieName, const char *cookieDomain);
QTRESTWRAPPERCAPISHARED_EXPORT const char **getOriginalCookieStringsByNameAndDomainAndPath(int64_t index, const char *cookieName, const char *cookieDomain, const char *cookiePath);
QTRESTWRAPPERCAPISHARED_EXPORT const char **getAllCookieStrings(int64_t index);

// static utilities
// generates a unique random string, unique only, when only this function will be used to generate the random string
static const char *randomString(int length = 12);
// utility function to split a cookie string into individual raw cookies
static requestHeader cookieSplit(const char *cookieString);

QTRESTWRAPPERCAPISHARED_EXPORT void registerAddWebView(int64_t index, void (*ptr)(void *));
QTRESTWRAPPERCAPISHARED_EXPORT void registerClosed(int64_t index, void (*ptr)());
QTRESTWRAPPERCAPISHARED_EXPORT void registerAuthenticated(int64_t index, void (*ptr)(struct qurlCAPI));
QTRESTWRAPPERCAPISHARED_EXPORT void registerAuthenticatedContent(int64_t index, void (*ptr)(struct qurlCAPI, const char *));
QTRESTWRAPPERCAPISHARED_EXPORT void registerAuthenticationCheck(int64_t index, bool (*ptr)(struct qurlCAPI));
QTRESTWRAPPERCAPISHARED_EXPORT void registerAuthenticationCheckContent(int64_t index, void (*ptr)(struct qurlCAPI, const char *));
QTRESTWRAPPERCAPISHARED_EXPORT void registerCertificateError(int64_t index, void (*ptr)(struct QRestWrapperCertificateError_CAPI));
QTRESTWRAPPERCAPISHARED_EXPORT void registerScriptEnded(int64_t index, void (*ptr)(const char *));
QTRESTWRAPPERCAPISHARED_EXPORT void registerAuthenticationFailure(int64_t index, void (*ptr)(struct qurlCAPI, const char *, int));
QTRESTWRAPPERCAPISHARED_EXPORT void registerNotFound(int64_t index, void (*ptr)(struct qurlCAPI, const char *));
QTRESTWRAPPERCAPISHARED_EXPORT void registerContentReady(int64_t index, void (*ptr)(struct qurlCAPI, const char *));

#endif // QTRESTWRAPPERCAPI_H
