#include "QNetworkAccessManagerWrapper.hpp"

QNetworkReply* QNetworkAccessManagerWrapper::get(const QNetworkRequest& request, const QByteArray& data)
{
    return manager.sendCustomRequest(request, "GET", data);
}
