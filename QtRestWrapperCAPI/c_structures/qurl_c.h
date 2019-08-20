#ifndef QURL_C_H
#define QURL_C_H

#include "../qtrestwrappercapi_global.h"

struct qurlCAPI {
    void *originalQUrl;
    char *urlData;
};

QTRESTWRAPPERCAPISHARED_EXPORT struct qurlCAPI qurlCapiFromString(char *url);

#endif // QURL_C_H
