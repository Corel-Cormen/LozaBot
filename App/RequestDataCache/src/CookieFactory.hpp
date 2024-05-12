#pragma once

#include "RequestDataCache.hpp"

class CookiesFactory
{
public:
    static QList<CookieStorage> createCookieStorage(const QByteArray& cookieData);

private:

    static QStringList convert2StringList(const QList<QByteArray>& list);

    static void assignMetadataAndData(QList<QString>& fieldCookie, CookieStorage& resultFactory);

    static void searchAndMarkCookieField(QList<QString>& fieldCookie, const QString field, bool& fieldFactory);

    static int findNameField(QList<QString>& fieldCookie, QString field);

    static void assignMetadata(CookieStorage& fieldCookie, const QString& metadata, const QString& data);
};
