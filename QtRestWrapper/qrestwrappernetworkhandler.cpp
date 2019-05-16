#include "qrestwrappernetworkhandler.h"
#include "qrestwrapperutils.h"

#include <QNetworkRequest>

using QtRestWrapper::QRestWrapperNetworkHandler;

QRestWrapperNetworkHandler::QRestWrapperNetworkHandler(QObject *parent)
    : QObject(parent),
      m_networkAccessManager()
{
    // connect the finished signal of the QNetworkAccessManager to the waitForCustomRequest slot
    // to be able to capture the result of the rest requests
    this->connect(&this->m_networkAccessManager, &QNetworkAccessManager::finished, this, &QRestWrapperNetworkHandler::waitForCustomRequest);
}

/*
 * rest sending functions
 */

void QRestWrapperNetworkHandler::sendGetRequest(const QUrl &url, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, QSharedPointer<QByteArray>::create(""), "GET", "", header);
}

void QRestWrapperNetworkHandler::sendPutRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, QSharedPointer<QByteArray>::create(payload), "PUT", contentType, header);
}

void QRestWrapperNetworkHandler::sendPutRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, payload, "PUT", contentType, header);
}

void QRestWrapperNetworkHandler::sendDeleteRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, QSharedPointer<QByteArray>::create(payload), "DELETE", contentType, header);
}

void QRestWrapperNetworkHandler::sendDeleteRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, payload, "DELETE", contentType, header);
}

void QRestWrapperNetworkHandler::sendOptionsRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, QSharedPointer<QByteArray>::create(payload), "OPTIONS", contentType, header);
}

void QRestWrapperNetworkHandler::sendOptionsRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, payload, "OPTIONS", contentType, header);
}

void QRestWrapperNetworkHandler::sendPostRequest(const QUrl &url, const QByteArray &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, QSharedPointer<QByteArray>::create(payload), "POST", contentType, header);
}

void QRestWrapperNetworkHandler::sendPostRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &contentType, const QMap<QString, QString> &header)
{
    return this->sendRequest(url, payload, "POST", contentType, header);
}

void QRestWrapperNetworkHandler::sendJsonRequest(const QUrl &url, const QJsonDocument &json, const QString &verb, const QMap<QString, QString> &header)
{
    QSharedPointer<QByteArray> jsonData = QSharedPointer<QByteArray>::create(json.toJson());
    return this->sendRequest(url, jsonData, verb, "application/json", header);
}

void QRestWrapperNetworkHandler::sendRequest(const QUrl &url, const QSharedPointer<QByteArray> &payload, const QString &verb, const QString &contentType, const QMap<QString, QString> &header)
{
    QByteArray postDataSize = QByteArray::number(payload->size());

    QNetworkRequest request;

    request.setUrl(url);
    if(contentType.length() > 0) {
        request.setRawHeader("Content-Type", contentType.toUtf8());
    }
    if(payload->size() > 0) {
        request.setRawHeader("Content-Length", postDataSize);
    }

    for(QMap<QString, QString>::const_iterator it = header.begin();it != header.end();it++) {
        request.setRawHeader(it.key().toUtf8(), it->toUtf8());
    }

    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    QString registeredString = QRestWrapperUtils::randomString();
    m_registeredPayloadData[registeredString] = QPair<QString, QSharedPointer<QByteArray>>(contentType, payload);
    m_registeredCustomHeaders[registeredString] = header;
    QNetworkReply *reply = this->m_networkAccessManager.sendCustomRequest(request, QByteArray(verb.toUtf8()), *payload);
    reply->setProperty("registeredString", registeredString);
}

void QRestWrapperNetworkHandler::waitForCustomRequest(QNetworkReply *reply)
{
    QString registeredString = reply->property("registeredString").toString();
    int statusCode = reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute).toInt();
    switch(statusCode) {
        case 200: {
            // request was successful
            QByteArray content = reply->read(1024);
            content += reply->readAll();
            emit this->contentReady(reply->url(), content);
            cleanupRequest(registeredString);
            break;
        }
        case 301:
        case 302:
        case 303:
        case 305:
        case 307:
        case 308: {
            // redirect requested
            QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            QUrl redirectUrl;
            if(!possibleRedirectUrl.isNull() && possibleRedirectUrl.isValid() && possibleRedirectUrl != reply->request().url()) {
                QVariant customVerb = reply->request().attribute(QNetworkRequest::CustomVerbAttribute);
                sendRequest(possibleRedirectUrl.toUrl(),
                            m_registeredPayloadData[registeredString].second,
                            customVerb.toString(),
                            m_registeredPayloadData[registeredString].first,
                            m_registeredCustomHeaders[registeredString]);
            }
            break;
        }
        case 401:
        case 403: {
            // authentication failure
            QByteArray content = reply->read(1024);
            content += reply->readAll();
            emit this->authenticationFailure(reply->url(), content, statusCode);
            cleanupRequest(registeredString);
            break;
        }
        case 404: {
            // the url was not found
            QByteArray content = reply->read(1024);
            content += reply->readAll();
            emit this->notFound(reply->url(), content);
            cleanupRequest(registeredString);
            break;
        }
        default: {
            // some other not defined http header was returned
            qDebug() << "statusCode: " << statusCode;
            QByteArray content = reply->read(1024);
            content += reply->readAll();
            emit this->undefinedStatusCode(reply->url(), content, statusCode);
            cleanupRequest(registeredString);
            break;
        }
    }
    reply->deleteLater();
}

void QRestWrapperNetworkHandler::cleanupRequest(const QString &registeredString)
{
    typedef QMap<QString, QPair<QString, QSharedPointer<QByteArray>>>::iterator RegisteredPayloadConstIterator;
    typedef QMap<QString, QMap<QString, QString>>::iterator RegisteredCustomHeadersConstIterators;
    for(RegisteredPayloadConstIterator it = m_registeredPayloadData.begin(); it != m_registeredPayloadData.end();) {
        if(it.key() == registeredString) {
            it = m_registeredPayloadData.erase(it);
        } else {
            ++it;
        }
    }
    for(RegisteredCustomHeadersConstIterators it = m_registeredCustomHeaders.begin(); it != m_registeredCustomHeaders.end();) {
        if(it.key() == registeredString) {
            it = m_registeredCustomHeaders.erase(it);
        } else {
            ++it;
        }
    }
}
