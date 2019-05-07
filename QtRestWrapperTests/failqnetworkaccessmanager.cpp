#include "failqnetworkaccessmanager.h"

#include <QMetaObject>
#include <QNetworkReply>
#include <QObject>

FailQNetworkAccessManager::FailQNetworkAccessManager(QObject *parent) : QNetworkAccessManager(parent)
{

}

QNetworkReply *FailQNetworkAccessManager::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    QNetworkReply* rep = QNetworkAccessManager::createRequest(op, request, outgoingData);
    // Queue the abort to occur from main loop
    QMetaObject::invokeMethod(rep, "abort", Qt::QueuedConnection);

    return rep;
}
