#include <gtest/gtest.h>

#include "CookieStorage.hpp"

namespace
{

template<typename T>
void compare(const CookieStorage<T>& soragePattern, const CookieStorage<T>& storageItem)
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

template<typename T>
T createCookieRawData(const CookieStorage<T>& pattern, bool isUpperFields)
{
    const T delimiter = "; ";
    auto convertField = [isUpperFields](const T field) -> T {
        if(isUpperFields)
        {
            return field;
        }
        else
        {
            return field.toLower();
        }
    };

    T result = pattern.metadata + "=" + pattern.data + delimiter;
    result += (convertField("Path") + "=" + pattern.path + delimiter);
    if(!pattern.expires.isEmpty())
    {
        result += (convertField("Expires") + "=" + pattern.expires + delimiter);
    }
    if(!pattern.domain.isEmpty())
    {
        result += (convertField("Domain") + "=" + pattern.domain + delimiter);
    }
    if(pattern.maxAge != 0)
    {
        result += (convertField("Max-Age") + "=" + T::number(pattern.maxAge) + delimiter);
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

typedef QList<QPair<CookieStorage<QString>, bool>> CookieStorageTestType;
class DataFactoryPositiveTest : public ::testing::TestWithParam<CookieStorageTestType>
{

};

TEST_P(DataFactoryPositiveTest, factoryTest)
{
    auto cookieStorageList = GetParam();
    QString cookieRawData;
    foreach(const auto& cookieTestPair, cookieStorageList)
    {
        if(!cookieRawData.isEmpty())
        {
            cookieRawData += '\n';
        }
        cookieRawData += createCookieRawData(cookieTestPair.first, cookieTestPair.second);
    }

    QList<CookieStorage<QString>> cookieStorageResult = CookiesFactory::createCookieStorage(std::move(cookieRawData));
    ASSERT_EQ(cookieStorageList.length(), cookieStorageResult.length());
    for(int i = 0; i < cookieStorageResult.length(); ++i)
    {
        compare(cookieStorageList.at(i).first, cookieStorageResult.at(0));
    }
}

INSTANTIATE_TEST_SUITE_P(DataFactoryParamTest, DataFactoryPositiveTest,
     ::testing::Values(
        CookieStorageTestType{
            {CookieStorage<QString>{
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
            {CookieStorage<QString>{
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
            {CookieStorage<QString>{
                .metadata = "metadata",
                .data = "data",
                .path = "path",
                .domain = "domain",
                .expires = "expires",
                .maxAge = 1000,
                .secure = true,
                .httpOnly = true
            }, true},
            {CookieStorage<QString>{
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
            {CookieStorage<QString>{
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
            {CookieStorage<QString>{
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
            {CookieStorage<QString>{
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
            {CookieStorage<QString>{
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
            {CookieStorage<QString>{
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
            {CookieStorage<QString>{
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
    QString emptyStr;
    auto result = CookiesFactory::createCookieStorage(std::move(emptyStr));
    EXPECT_EQ(0, result.length());
}

TEST_F(DataFactoryNegativeTest, metadataParseNotSupported)
{
    CookieStorage<QString> cookie = {
        .metadata = "metadata",
        .data = "data",
        .path = "path",
        .domain = "domain",
        .expires = "expires",
        .maxAge = 1000,
        .secure = true,
        .httpOnly = true
    };
    QString cookieRawData = createCookieRawData(cookie, false);
    cookieRawData += "; AddingMetadata";

    auto result = CookiesFactory::createCookieStorage(std::move(cookieRawData));
    EXPECT_EQ(1, result.length());
    compare(cookie, result.at(0));
}

TEST_F(DataFactoryNegativeTest, metadataAndDataParseNotSupported)
{
    CookieStorage<QString> cookie = {
        .metadata = "metadata",
        .data = "data",
        .path = "path",
        .domain = "domain",
        .expires = "expires",
        .maxAge = 1000,
        .secure = true,
        .httpOnly = true
    };
    QString cookieRawData = createCookieRawData(cookie, false);
    cookieRawData += "; AddingMetadata=AddingData";

    auto result = CookiesFactory::createCookieStorage(std::move(cookieRawData));
    EXPECT_EQ(1, result.length());
    compare(cookie, result.at(0));
}
