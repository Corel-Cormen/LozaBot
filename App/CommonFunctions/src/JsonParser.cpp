#include <QJsonObject>
#include <QNetworkRequest>
#include <QByteArray>
#include <QJsonDocument>

#include "JsonParser.hpp"

QNetworkRequest JsonParser::parseRequest(const QString& url, const QByteArray& cookieData)
{
    QNetworkRequest request;

    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "*/*");
    request.setRawHeader("Accept-Encoding", "gzip, deflate, br, zstd");
    request.setRawHeader("Cookie", cookieData);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/125.0.0.0 Safari/537.36");

    return request;
}
