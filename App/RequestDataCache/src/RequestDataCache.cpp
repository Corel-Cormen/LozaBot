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

QJsonObject RequestDataCache::getCookies() const
{
    QJsonObject json;

    for(size_t i = 0; i < cookiesStorage->getSize(); ++i)
    {
        const CookieData cookie = cookiesStorage->getCookie(i);
        json[cookie.metadata] = cookie.data;
    }

    return json;
}
