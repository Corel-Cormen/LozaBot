#pragma once

template <typename T>
class QList;
struct CookieData;
class QStringList;
class QByteArray;
class QString;

class CookiesFactory
{
public:
    static QList<CookieData> createCookieStorage(const QByteArray& cookieData);

private:

    static QStringList convert2StringList(const QList<QByteArray>& list);

    static void assignMetadataAndData(QList<QString>& fieldCookie, CookieData& resultFactory);

    static void searchAndMarkCookieField(QList<QString>& fieldCookie, const QString field, bool& fieldFactory);

    static int findNameField(QList<QString>& fieldCookie, QString field);

    static void assignMetadata(CookieData& fieldCookie, const QString& metadata, const QString& data);
};
