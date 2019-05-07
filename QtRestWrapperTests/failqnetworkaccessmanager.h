#ifndef FAILQNETWORKACCESSMANAGER_H
#define FAILQNETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>
#include <QObject>



class FailQNetworkAccessManager : public QNetworkAccessManager
{
public:
    FailQNetworkAccessManager(QObject *parent = Q_NULLPTR);

    // QNetworkAccessManager interface
protected:
    QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData);
};

#endif // FAILQNETWORKACCESSMANAGER_H
