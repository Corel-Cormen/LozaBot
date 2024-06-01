#pragma once

#include <QNetworkAccessManager>

class QNetworkAccessManagerWrapper
{
public:
    QNetworkAccessManagerWrapper() = default;

    ~QNetworkAccessManagerWrapper() = default;

    virtual QNetworkReply* get(const QNetworkRequest& request);

private:
    QNetworkAccessManager manager;
};
