#pragma once

class QJsonObject;
class QNetworkRequest;
class QByteArray;
class QString;

namespace JsonParser
{

QNetworkRequest parseRequest(const QString& url, const QByteArray& cookieData);

} // end namespace JsonParser
