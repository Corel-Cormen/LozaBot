#pragma once

#include <optional>
#include <QString>

#include "RequestDriver.hpp"

class RequestCache
{
    RequestCache(RequestCache&) = delete;

    RequestCache& operator=(const RequestCache&) = delete;

public:
    RequestCache();

    ~RequestCache() = default;

    std::optional<RequestDriver::MetadataList> getHeaders();

    void putHeader(const QString& metadata, const QString& data);

private:
    RequestDriver::MetadataList headerList;
};
