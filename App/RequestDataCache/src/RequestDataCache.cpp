#include "RequestDataCache.hpp"
#include "CookieFactory.hpp"

RequestDataCache::RequestDataCache()
{

}

void RequestDataCache::updateCookies(QByteArray cookiesData)
{
    CookiesFactory::createCookieStorage(cookiesData);
}
