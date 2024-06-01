#include <gtest/gtest.h>

#include <RequestDataCache.hpp>
#include "JsonUtils.hpp"

class RequestDataCacheTest : public ::testing::Test
{
protected:
    RequestDataCache dataCache;
};

TEST_F(RequestDataCacheTest, getEmptyCookiesTest)
{
    QByteArray jsonCookie = dataCache.getRawCookies();
    EXPECT_TRUE(jsonCookie.isEmpty());
}

TEST_F(RequestDataCacheTest, getOnePartCookiesTest)
{
    CookieData cookie{ .metadata = "metadata", .data = "data"};
    QByteArray expectCookie{"metadata=data"};
    dataCache.updateCookies(JsonUtils::createCookieRawData(cookie, false));

    QByteArray cookieArray = dataCache.getRawCookies();
    EXPECT_STREQ(cookieArray, expectCookie);
}

TEST_F(RequestDataCacheTest, getMultiplePartCookiesTest)
{
    dataCache.updateCookies(JsonUtils::createCookieRawData({ .metadata = "metadata1", .data = "data1"}, false));
    dataCache.updateCookies(JsonUtils::createCookieRawData({ .metadata = "metadata2", .data = "data2"}, false));
    QByteArray expectCookie{"metadata1=data1; metadata2=data2"};

    QByteArray cookieArray = dataCache.getRawCookies();
    EXPECT_STREQ(cookieArray, expectCookie);
}
