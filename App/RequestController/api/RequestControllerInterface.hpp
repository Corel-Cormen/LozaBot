#pragma once

#include "CommonTypes.hpp"

class RequestControllerInterface
{
public:
    RequestControllerInterface() = default;

    virtual ~RequestControllerInterface() = default;

    virtual Error_Code_T enterStartWebsite() = 0;
};
