#pragma once

#include <QtGlobal>
#include <QString>
#include <QList>

struct CookieData
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

struct CookieStorage
{
public:
    int indexOf(const CookieData& _cookie) const;

    void update(const CookieData& _cookie);

private:
    QList<CookieData> cookies;
};
