#pragma once

#include <QNetworkReply>

#include "RequestDriverInterface.hpp"
#include "QEventLoopWrapper.hpp"
#include "QNetworkAccessManagerWrapper.hpp"

class RequestCache;

class RequestDriver : public RequestDriverInterface
{
public:

    explicit RequestDriver(QEventLoopWrapper& _eventLoop,
                           QNetworkAccessManagerWrapper& _networkManager);

    virtual ~RequestDriver();

    virtual Error_Code_T GET(const QUrl& url) override;

    virtual bool getResponseHeader(MetadataList& header) override;

private:

    Error_Code_T configureGET(const QUrl& url);

    Error_Code_T receiveResponse();

    Error_Code_T wrapHeader();

    QNetworkReply* networkReply;

    QNetworkAccessManagerWrapper& networkManager;

    QEventLoopWrapper& eventLoop;

    RequestCache* requestCache;
};
