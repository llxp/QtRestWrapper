#ifndef QRESTWRAPPERNETWORKHANDLER_H
#define QRESTWRAPPERNETWORKHANDLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMap>
#include <QJsonDocument>

namespace QtRestWrapper {
class QRestWrapperNetworkHandler : public QObject
{
    Q_OBJECT
public:
    explicit QRestWrapperNetworkHandler(QObject *parent = nullptr);

public slots:
    void sendGetRequest(const QUrl &url, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendPutRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendPutRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendDeleteRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendDeleteRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendOptionsRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendOptionsRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendPostRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendPostRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendJsonRequest(const QUrl &url, const QJsonDocument &json, const QString &verb, const QMap<QString, QString> &header = QMap<QString, QString>());
    void sendRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &verb, const QString &contentType, const QMap<QString, QString> &header = QMap<QString, QString>());

signals:
    // these signals will be called, once there has been a reply from a rest request
    // check for the url, to see, if the signal matches the right request
    void contentReady(const QUrl &url, const QByteArray &content); // status 200
    void authenticationFailure(const QUrl &url, const QByteArray &content, int statusCode); // status 40*
    void notFound(const QUrl &url, const QByteArray &content); // status 404
    void undefinedStatusCode(const QUrl &url, const QByteArray &content, int statusCode); // every other status code

private slots:
    void waitForCustomRequest(QNetworkReply *reply);
    void cleanupRequest(const QString &registeredString);

private:
    QNetworkAccessManager m_networkAccessManager;
    QMap<QString, QPair<QString, QSharedPointer<QByteArray>>> m_registeredPayloadData;
    QMap<QString, QMap<QString, QString>> m_registeredCustomHeaders;
};
}  // namespace QtRestWrapper

#endif // QRESTWRAPPERNETWORKHANDLER_H
