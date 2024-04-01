#pragma once

#include "RequestControllerInterface.hpp"
#include "RequestDriverInterface.hpp"

class CookieCache;

class RequestController : public RequestControllerInterface
{
public:
    explicit RequestController(RequestDriverInterface& _requestDrv);

    virtual ~RequestController();

    virtual Error_Code_T enterStartWebsite() override;

private:

    RequestDriverInterface& requestDrv;

    CookieCache* cookieCache;
};
