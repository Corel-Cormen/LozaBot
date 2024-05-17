#include "RequestDataCache.hpp"
#include "CookieFactory.hpp"
#include "CookieStorage.hpp"

RequestDataCache::RequestDataCache() :
    cookiesStorage{new CookieStorage}
{

}

RequestDataCache::~RequestDataCache()
{
    delete cookiesStorage;
}

void RequestDataCache::updateCookies(const QByteArray& cookiesData)
{
    QList<CookieData> newCookiesData = CookiesFactory::createCookieStorage(cookiesData);
    foreach(const CookieData& cookie, newCookiesData)
    {
        cookiesStorage->update(cookie);
    }
}
