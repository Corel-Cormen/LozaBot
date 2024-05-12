#pragma once

#include <QtGlobal>
#include <QByteArray>
#include <QList>

struct CookieStorage
{
    QString metadata;
    QString data;
    QString path;
    QString domain;
    QString expires;
    quint32 maxAge;
    bool secure;
    bool httpOnly;
};

class RequestDataCache
{
public:
    RequestDataCache();

    void updateCookies(QByteArray cookiesData);

private:
    QList<CookieStorage> cookiesStorage{};
};
