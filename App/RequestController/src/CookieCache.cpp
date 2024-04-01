#include "CookieCache.hpp"

CookieCache::CookieCache()
{

}

void CookieCache::parseCookies(QString&& cookies)
{
    cookiesStorage = CookiesFactory::createCookieStorage<QString>(std::move(cookies));
}
