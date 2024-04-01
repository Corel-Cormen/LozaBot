#pragma once

#include <QtGlobal>
#include <QList>
#include <fmt/core.h>

template <typename T>
struct CookieStorage
{
    T metadata;
    T data;
    T path;
    T domain;
    T expires;
    quint32 maxAge;
    bool secure;
    bool httpOnly;
};

class CookiesFactory
{
public:

    template<typename T>
    static QList<CookieStorage<T>> createCookieStorage(T&& cookieData)
    {
        QList<CookieStorage<T>> resultFactoryList;

        if(cookieData.isEmpty())
        {
            return resultFactoryList;
        }

        auto&& cookies = cookieData.split('\n');

        std::for_each(cookies.begin(), cookies.end(), [&resultFactoryList](T& cookie) {
            auto&& fieldCookie = cookie.split("; ");
            CookieStorage<T> resultFactory{};

            auto&& dataAndMetadataCookie = fieldCookie.at(0).split('=');
            resultFactory.metadata = std::move(dataAndMetadataCookie.at(0));
            resultFactory.data = std::move(dataAndMetadataCookie.at(1));
            fieldCookie.pop_front();

            searchAndMarkCookieField(fieldCookie, "Secure", resultFactory.secure);
            searchAndMarkCookieField(fieldCookie, "HttpOnly", resultFactory.httpOnly);

            foreach(auto& field, fieldCookie)
            {
                auto&& dataAndMetadata = field.split('=');
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
        });

        return resultFactoryList;
    }

private:

    template<typename T>
    static void searchAndMarkCookieField(QList<T>& fieldCookie, const QByteArray field, bool& fieldFactory)
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

    template<typename T>
    static int findNameField(QList<T>& fieldCookie, QByteArray field)
    {
        int findResult = fieldCookie.indexOf(field);
        if(findResult == -1)
        {
            findResult = fieldCookie.indexOf(field.toLower());
        }
        return findResult;
    }

    template<typename T>
    static void assignMetadata(CookieStorage<T>& fieldCookie, const T& metadata, const T& data)
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
};
