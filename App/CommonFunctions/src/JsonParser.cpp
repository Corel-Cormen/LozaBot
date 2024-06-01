#include <QJsonObject>
#include <QNetworkRequest>
#include <QByteArray>
#include <QJsonDocument>

#include "JsonParser.hpp"

QByteArray JsonParser::parseJson(const QJsonObject& data)
{
    return QJsonDocument{data}.toJson();
}

QNetworkRequest JsonParser::parseRequest(const QString& url, const QByteArray& jsonData)
{
    QNetworkRequest request;

    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(jsonData.size()));

    return request;
}
