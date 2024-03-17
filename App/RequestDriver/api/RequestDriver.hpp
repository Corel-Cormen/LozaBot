#pragma once

#include <QNetworkReply>

#include "RequestDriverInterface.hpp"

class RequestDriver : public RequestDriverInterface
{
public:

    explicit RequestDriver();

    virtual ~RequestDriver() = default;

    virtual void GET() override;

private:
    void reciveCall();

    void receiveResponse();

    QNetworkReply* networkReply;
};
