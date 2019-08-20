#include "qurl_c.h"
#include "stdlib.h"

#include <QUrl>
#include <QString>

struct qurlCAPI *newObj(struct qurlCAPI *qurl_obj)
{
    struct qurlCAPI * obj = nullptr;
    if(qurl_obj == nullptr) {
        obj = static_cast<struct qurlCAPI *>(malloc(sizeof(struct qurlCAPI)));
    } else {
        obj = qurl_obj;
    }
    return obj;
}

struct qurlCAPI qurlCapiFromString(char *url)
{
    struct qurlCAPI obj;
    QUrl *urlObj = new QUrl(QString::fromUtf8(url));
    QString strData = urlObj->toString();
    obj.urlData = static_cast<char *>(malloc(sizeof(char) * static_cast<size_t>(strData.size()) + 1));
    strcpy_s(obj.urlData, static_cast<size_t>(strData.size()), strData.toUtf8().data());
    obj.urlData[strData.size()] = '\0';
    obj.originalQUrl = urlObj;
    return obj;
}
