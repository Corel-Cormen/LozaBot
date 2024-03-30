#include "RequestCache.hpp"

RequestCache::RequestCache()
{

}

std::optional<RequestDriver::MetadataList> RequestCache::getHeaders()
{
    if(!headerList.isEmpty())
    {
        return std::move(headerList);
    }
    return std::nullopt;
}

void RequestCache::putHeader(const QString& metadata, const QString& data)
{
    headerList.push_back({metadata, data});
}
