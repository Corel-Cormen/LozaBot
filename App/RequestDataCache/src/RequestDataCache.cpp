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

QByteArray RequestDataCache::getRawCookies() const
{
    QByteArray cookiesArray{};

    for(size_t i = 0; i < cookiesStorage->getSize(); ++i)
    {
        const CookieData cookie = cookiesStorage->getCookie(i);
        cookiesArray += (cookie.metadata + "=" + cookie.data).toUtf8();

        if(i < cookiesStorage->getSize()-1)
        {
            cookiesArray += "; ";
        }
    }

    return cookiesArray;
}
