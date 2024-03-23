#pragma once

#include <QNetworkReply>

#include "RequestDriverInterface.hpp"
#include "QEventLoopWrapper.hpp"
#include "QNetworkAccessManagerWrapper.hpp"

class RequestDriver : public RequestDriverInterface
{
public:

    explicit RequestDriver(QEventLoopWrapper& _eventLoop,
                           QNetworkAccessManagerWrapper& _networkManager);

    virtual ~RequestDriver() = default;

    virtual Error_Code_T GET(const QUrl& url) override;

private:

    Error_Code_T configureGET(const QUrl& url);

    Error_Code_T receiveResponse();

    QNetworkReply* networkReply;

    QNetworkAccessManagerWrapper& networkManager;

    QEventLoopWrapper& eventLoop;
};
