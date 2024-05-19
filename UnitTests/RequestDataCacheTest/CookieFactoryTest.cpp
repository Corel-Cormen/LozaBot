#include <gtest/gtest.h>

#include "CookieFactory.hpp"
#include "CookieStorage.hpp"
#include "JsonUtils.hpp"

namespace
{

void compare(const CookieData& soragePattern, const CookieData& storageItem)
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

} // end namespace

typedef QList<QPair<CookieData, bool>> CookieStorageTestType;
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
        cookieRawData += JsonUtils::createCookieRawData(cookieTestPair.first, cookieTestPair.second);
    }

    QList<CookieData> cookieStorageResult = CookiesFactory::createCookieStorage(cookieRawData);
    ASSERT_EQ(cookieStorageList.length(), cookieStorageResult.length());
    for(int i = 0; i < cookieStorageResult.length(); ++i)
    {
        compare(cookieStorageList.at(i).first, cookieStorageResult.at(0));
    }
}

INSTANTIATE_TEST_SUITE_P(DataFactoryParamTest, DataFactoryPositiveTest,
     ::testing::Values(
        CookieStorageTestType{
            {CookieData {
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
            {CookieData {
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
            {CookieData {
                .metadata = "metadata",
                .data = "data",
                .path = "path",
                .domain = "domain",
                .expires = "expires",
                .maxAge = 1000,
                .secure = true,
                .httpOnly = true
            }, true},
            {CookieData {
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
            {CookieData {
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
            {CookieData {
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
            {CookieData {
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
            {CookieData {
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
            {CookieData {
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
            {CookieData {
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
    CookieData cookie = {
        .metadata = "metadata",
        .data = "data",
        .path = "path",
        .domain = "domain",
        .expires = "expires",
        .maxAge = 1000,
        .secure = true,
        .httpOnly = true
    };
    QByteArray cookieRawData = JsonUtils::createCookieRawData(cookie, false);
    cookieRawData += "; AddingMetadata";

    auto result = CookiesFactory::createCookieStorage(cookieRawData);
    ASSERT_EQ(1, result.length());
    compare(cookie, result.at(0));
}

TEST_F(DataFactoryNegativeTest, metadataAndDataParseNotSupported)
{
    CookieData cookie = {
        .metadata = "metadata",
        .data = "data",
        .path = "path",
        .domain = "domain",
        .expires = "expires",
        .maxAge = 1000,
        .secure = true,
        .httpOnly = true
    };
    QByteArray cookieRawData = JsonUtils::createCookieRawData(cookie, false);
    cookieRawData += "; AddingMetadata=AddingData";

    auto result = CookiesFactory::createCookieStorage(cookieRawData);
    ASSERT_EQ(1, result.length());
    compare(cookie, result.at(0));
}
