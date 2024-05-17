#pragma once

class QByteArray;
struct CookieStorage;

class RequestDataCache
{
public:
    RequestDataCache();

    virtual ~RequestDataCache();

    void updateCookies(const QByteArray& cookiesData);

private:
    CookieStorage* cookiesStorage;
};
