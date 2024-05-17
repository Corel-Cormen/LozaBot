#include "CookieStorage.hpp"

int CookieStorage::indexOf(const CookieData& _cookie) const
{
    auto it = std::find_if(cookies.begin(), cookies.end(),[&_cookie](const CookieData& cookieElement) {
        return cookieElement.metadata == _cookie.metadata;
    });

    if(it != cookies.end())
    {
        return std::distance(cookies.begin(), it);
    }
    return -1;
}

void CookieStorage::update(const CookieData& _cookie)
{
    if(int idx = indexOf(_cookie); idx != -1)
    {
        if(cookies[idx].data != _cookie.data)
        {
            cookies[idx].data = _cookie.data;
            cookies[idx].expires = _cookie.expires;
        }
    }
    else
    {
        cookies.append(_cookie);
    }
}
