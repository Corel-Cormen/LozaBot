#pragma once

#include "CommonTypes.hpp"

class QString;

class RequestControllerInterface
{
public:
    RequestControllerInterface() = default;

    virtual ~RequestControllerInterface() = default;

    virtual Error_Code_T enterWebsite(const QString& url) = 0;
};
