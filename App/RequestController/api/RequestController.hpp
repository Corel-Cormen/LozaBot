#pragma once

#include "RequestControllerInterface.hpp"
#include "RequestDriverInterface.hpp"
#include "RequestDataCache.hpp"

class RequestController : public RequestControllerInterface
{
public:
    explicit RequestController(RequestDriverInterface& _requestDrv);

    virtual ~RequestController() = default;

    virtual Error_Code_T enterWebsite(const QString& url) override;

private:

    RequestDriverInterface& requestDrv;

    RequestDataCache cookieCache;
};
