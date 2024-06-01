#include "QNetworkAccessManagerWrapper.hpp"

QNetworkReply* QNetworkAccessManagerWrapper::get(const QNetworkRequest& request)
{
    return manager.get(request);
}
