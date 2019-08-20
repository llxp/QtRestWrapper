#ifndef QNETWORKCOOKIE_CAPI_H
#define QNETWORKCOOKIE_CAPI_H

struct qnetworkcookieCAPI {
    char *value;
};

struct qnetworkcookieListCAPI {
    qnetworkcookieCAPI *cookies;
    unsigned long long cookieCount;
};

#endif // QNETWORKCOOKIE_CAPI_H
