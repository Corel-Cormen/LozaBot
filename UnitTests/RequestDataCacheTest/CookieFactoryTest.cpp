#include <gtest/gtest.h>

#include "CookieFactory.hpp"

namespace
{

void compare(const CookieStorage& soragePattern, const CookieStorage& storageItem)
{
    EXPECT_STREQ(soragePattern.metadata.toStdString().c_str(), storageItem.metadata.toStdString().c_str());
    EXPECT_STREQ(soragePattern.data.toStdString().c_str(), storageItem.data.toStdString().c_str());
    EXPECT_STREQ(soragePattern.path.toStdString().c_str(), storageItem.path.toStdString().c_str());
    EXPECT_STREQ(soragePattern.domain.toStdString().c_str(), storageItem.domain.toStdString().c_str());
    EXPECT_STREQ(soragePattern.expires.toStdString().c_str(), storageItem.expires.toStdString().c_str());
    EXPECT_EQ(soragePattern.maxAge, storageItem.maxAge);
    EXPECT_EQ(soragePattern.secure, storageItem.secure);
    EXPECT_EQ(soragePattern.httpOnly, storageItem.httpOnly);
}

QByteArray createCookieRawData(const CookieStorage& pattern, bool isUpperFields)
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

} // end namespace

typedef QList<QPair<CookieStorage, bool>> CookieStorageTestType;
class DataFactoryPositiveTest : public ::testing::TestWithParam<CookieStorageTestType>
{

};

TEST_P(DataFactoryPositiveTest, factoryTest)
{
    auto cookieStorageList = GetParam();
    QByteArray cookieRawData;
    foreach(const auto& cookieTestPair, cookieStorageList)
    {
        if(!cookieRawData.isEmpty())
        {
            cookieRawData += '\n';
        }
        cookieRawData += createCookieRawData(cookieTestPair.first, cookieTestPair.second);
    }

    QList<CookieStorage> cookieStorageResult = CookiesFactory::createCookieStorage(cookieRawData);
    ASSERT_EQ(cookieStorageList.length(), cookieStorageResult.length());
    for(int i = 0; i < cookieStorageResult.length(); ++i)
    {
        compare(cookieStorageList.at(i).first, cookieStorageResult.at(0));
    }
}

INSTANTIATE_TEST_SUITE_P(DataFactoryParamTest, DataFactoryPositiveTest,
     ::testing::Values(
        CookieStorageTestType{
            {CookieStorage {
                .metadata = "metadata",
                .data = "data",
                .path = "path",
                .domain = "domain",
                .expires = "expires",
                .maxAge = 1000,
                .secure = true,
                .httpOnly = true
            }, true}
         },
         CookieStorageTestType{
            {CookieStorage {
                .metadata = "metadata",
                .data = "data",
                .path = "path",
                .domain = "domain",
                .expires = "expires",
                .maxAge = 1000,
                .secure = true,
                .httpOnly = true
            }, false}
         },
         CookieStorageTestType{
            {CookieStorage {
                .metadata = "metadata",
                .data = "data",
                .path = "path",
                .domain = "domain",
                .expires = "expires",
                .maxAge = 1000,
                .secure = true,
                .httpOnly = true
            }, true},
            {CookieStorage {
                .metadata = "metadata",
                .data = "data",
                .path = "path",
                .domain = "domain",
                .expires = "expires",
                .maxAge = 1000,
                .secure = true,
                .httpOnly = true
            }, false},
        },
        CookieStorageTestType{
            {CookieStorage {
                .metadata = "metadata",
                .data = "data",
                .domain = "domain",
                .expires = "expires",
                .maxAge = 1000,
                .secure = true,
                .httpOnly = true
            }, false}
        },
        CookieStorageTestType{
            {CookieStorage {
                .metadata = "metadata",
                .data = "data",
                .path = "path",
                .expires = "expires",
                .maxAge = 1000,
                .secure = true,
                .httpOnly = true
            }, false}
        },
        CookieStorageTestType{
            {CookieStorage {
                .metadata = "metadata",
                .data = "data",
                .path = "path",
                .domain = "domain",
                .maxAge = 1000,
                .secure = true,
                .httpOnly = true
            }, false}
        },
        CookieStorageTestType{
            {CookieStorage {
                .metadata = "metadata",
                .data = "data",
                .path = "path",
                .domain = "domain",
                .expires = "expires",
                .maxAge = 0,
                .secure = true,
                .httpOnly = true
            }, false}
        },
        CookieStorageTestType{
            {CookieStorage {
                .metadata = "metadata",
                .data = "data",
                .path = "path",
                .domain = "domain",
                .expires = "expires",
                .maxAge = 1000,
                .secure = false,
                .httpOnly = true
            }, false}
        },
        CookieStorageTestType{
            {CookieStorage {
                .metadata = "metadata",
                .data = "data",
                .path = "path",
                .domain = "domain",
                .expires = "expires",
                .maxAge = 1000,
                .secure = true,
                .httpOnly = false
            }, false}
        }
    )
);

class DataFactoryNegativeTest : public ::testing::Test
{

};

TEST_F(DataFactoryNegativeTest, parseEmptyRawData)
{
    QByteArray emptyStr;
    auto result = CookiesFactory::createCookieStorage(emptyStr);
    EXPECT_EQ(0, result.length());
}

TEST_F(DataFactoryNegativeTest, metadataParseNotSupported)
{
    CookieStorage cookie = {
        .metadata = "metadata",
        .data = "data",
        .path = "path",
        .domain = "domain",
        .expires = "expires",
        .maxAge = 1000,
        .secure = true,
        .httpOnly = true
    };
    QByteArray cookieRawData = createCookieRawData(cookie, false);
    cookieRawData += "; AddingMetadata";

    auto result = CookiesFactory::createCookieStorage(cookieRawData);
    ASSERT_EQ(1, result.length());
    compare(cookie, result.at(0));
}

TEST_F(DataFactoryNegativeTest, metadataAndDataParseNotSupported)
{
    CookieStorage cookie = {
        .metadata = "metadata",
        .data = "data",
        .path = "path",
        .domain = "domain",
        .expires = "expires",
        .maxAge = 1000,
        .secure = true,
        .httpOnly = true
    };
    QByteArray cookieRawData = createCookieRawData(cookie, false);
    cookieRawData += "; AddingMetadata=AddingData";

    auto result = CookiesFactory::createCookieStorage(cookieRawData);
    ASSERT_EQ(1, result.length());
    compare(cookie, result.at(0));
}
