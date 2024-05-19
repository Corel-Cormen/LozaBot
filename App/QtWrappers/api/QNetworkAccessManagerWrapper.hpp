#pragma once

#include <QNetworkAccessManager>

class QNetworkAccessManagerWrapper
{
public:
    QNetworkAccessManagerWrapper() = default;

    ~QNetworkAccessManagerWrapper() = default;

    virtual QNetworkReply* get(const QNetworkRequest& request, const QByteArray& data);

private:
    QNetworkAccessManager manager;
};
