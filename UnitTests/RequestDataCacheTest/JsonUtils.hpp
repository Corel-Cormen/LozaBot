#pragma once

#include <QByteArray>

#include "CookieStorage.hpp"

namespace JsonUtils {

static QByteArray createCookieRawData(const CookieData& pattern, bool isUpperFields)
{
    const QByteArray delimiter = "; ";
    auto convertField = [isUpperFields](const QByteArray field) -> QByteArray {
        if(isUpperFields)
        {
            return field;
        }
        else
        {
            return field.toLower();
        }
    };

    QByteArray result = QString(pattern.metadata + '=' + pattern.data + delimiter).toUtf8();
    result += (convertField("Path") + "=" + pattern.path + delimiter).toUtf8();
    if(!pattern.expires.isEmpty())
    {
        result += (convertField("Expires") + "=" + pattern.expires + delimiter).toUtf8();
    }
    if(!pattern.domain.isEmpty())
    {
        result += (convertField("Domain") + "=" + pattern.domain + delimiter).toUtf8();
    }
    if(pattern.maxAge != 0)
    {
        result += (convertField("Max-Age") + "=" + QByteArray::number(pattern.maxAge) + delimiter);
    }
    if(pattern.secure)
    {
        result += (convertField("Secure") + delimiter);
    }
    if(pattern.httpOnly)
    {
        result += (convertField("HttpOnly") + delimiter);
    }

    if((result.lastIndexOf(delimiter) + delimiter.length()) == result.length())
    {
        result.remove(result.lastIndexOf(delimiter), delimiter.length());
    }

    return result;
}

} // end JsonUtils namespace
