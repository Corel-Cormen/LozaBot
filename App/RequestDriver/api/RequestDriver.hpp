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

    virtual Error_Code_T GET(const QNetworkRequest& request, const QByteArray& data) override;

    virtual Error_Code_T getResponseHeader(const MetadataList*& header) override;

private:

    Error_Code_T configureGET(const QNetworkRequest& url, const QByteArray& data);

    Error_Code_T receiveResponse();

    QNetworkReply* networkReply{nullptr};

    QNetworkAccessManagerWrapper& networkManager;

    QEventLoopWrapper& eventLoop;
};
