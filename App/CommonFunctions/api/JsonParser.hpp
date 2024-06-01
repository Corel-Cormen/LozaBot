#pragma once

class QJsonObject;
class QNetworkRequest;
class QByteArray;
class QString;

namespace JsonParser
{

QByteArray parseJson(const QJsonObject& data);

QNetworkRequest parseRequest(const QString& url, const QByteArray& jsonData);

} // end namespace JsonParser
