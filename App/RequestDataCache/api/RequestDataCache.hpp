#pragma once

#include <QJsonObject>

class QByteArray;
struct CookieStorage;

class RequestDataCache
{
public:
    RequestDataCache();

    virtual ~RequestDataCache();

    void updateCookies(const QByteArray& cookiesData);

    QJsonObject getCookies() const;

private:
    CookieStorage* cookiesStorage;
};
