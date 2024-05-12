#include "CookieFactory.hpp"
#include "fmt/core.h"

QList<CookieStorage> CookiesFactory::createCookieStorage(const QByteArray& cookieData)
{
    QList<CookieStorage> resultFactoryList;

    if(cookieData.isEmpty())
    {
        return resultFactoryList;
    }

    foreach(const QString& cookie, convert2StringList(cookieData.split('\n')))
    {
        QList<QString> fieldCookie = cookie.split("; ");
        CookieStorage resultFactory{};

        assignMetadataAndData(fieldCookie, resultFactory);
        searchAndMarkCookieField(fieldCookie, "Secure", resultFactory.secure);
        searchAndMarkCookieField(fieldCookie, "HttpOnly", resultFactory.httpOnly);

        foreach(const auto& field, fieldCookie)
        {
            QList<QString> dataAndMetadata = field.split('=');
            if(dataAndMetadata.length() == 2)
            {
                assignMetadata(resultFactory, dataAndMetadata.at(0), dataAndMetadata.at(1));
            }
            else
            {
                fmt::println("field {} parse not supported", field.toStdString());
            }
        }

        resultFactoryList.append(resultFactory);
    }

    return resultFactoryList;
}

QStringList CookiesFactory::convert2StringList(const QList<QByteArray>& list)
{
    QStringList result;
    foreach(const auto& item, list)
    {
        result.append(QString::fromLocal8Bit(item));
    }
    return result;
}

void CookiesFactory::assignMetadataAndData(QList<QString>& fieldCookie, CookieStorage& resultFactory)
{
    QList<QString> dataAndMetadataCookie = fieldCookie.at(0).split('=');
    resultFactory.metadata = dataAndMetadataCookie.at(0);
    resultFactory.data = dataAndMetadataCookie.at(1);
    fieldCookie.pop_front();
}

void CookiesFactory::searchAndMarkCookieField(QList<QString>& fieldCookie, const QString field, bool& fieldFactory)
{
    int position = findNameField(fieldCookie, field);
    if(position != -1)
    {
        fieldFactory = true;
        fieldCookie.removeAt(position);
    }
    else
    {
        fieldFactory = false;
    }
}

int CookiesFactory::findNameField(QList<QString>& fieldCookie, QString field)
{
    int findResult = fieldCookie.indexOf(field);
    if(findResult == -1)
    {
        findResult = fieldCookie.indexOf(field.toLower());
    }
    return findResult;
}

void CookiesFactory::assignMetadata(CookieStorage& fieldCookie, const QString& metadata, const QString& data)
{
    if((metadata == "Path") || (metadata == "path"))
    {
        fieldCookie.path = data;
    }
    else if((metadata == "Domain") || (metadata == "domain"))
    {
        fieldCookie.domain = data;
    }
    else if((metadata == "Expires") || (metadata == "expires"))
    {
        fieldCookie.expires = data;
    }
    else if((metadata == "Max-Age") || (metadata == "max-age"))
    {
        fieldCookie.maxAge = data.toUInt();
    }
    else
    {
        fmt::println("metadata {} parse not supported", metadata.toStdString());
    }
}
