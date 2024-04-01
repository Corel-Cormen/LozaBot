#pragma once

#include <QList>

#include "CookieStorage.hpp"

class CookieCache
{
public:

    CookieCache();

    ~CookieCache() = default;

    void parseCookies(QString&& cookies);

private:
    QList<CookieStorage<QString>> cookiesStorage;
};
