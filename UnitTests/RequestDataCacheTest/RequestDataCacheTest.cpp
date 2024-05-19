#include <gtest/gtest.h>

#include <RequestDataCache.hpp>
#include "JsonUtils.hpp"

class RequestDataCacheTest : public ::testing::Test
{
protected:
    RequestDataCache dataCache;
};

TEST_F(RequestDataCacheTest, getEmptyJsonTest)
{
    QJsonObject jsonCookie = dataCache.getCookies();
    EXPECT_TRUE(jsonCookie.isEmpty());
}

TEST_F(RequestDataCacheTest, getJsonTest)
{
    CookieData cookie{ .metadata = "metadata", .data = "data"};
    dataCache.updateCookies(JsonUtils::createCookieRawData(cookie, false));

    QJsonObject jsonCookie = dataCache.getCookies();
    ASSERT_EQ(jsonCookie.size(), 1);

    QVariantMap jsonMap = jsonCookie.toVariantMap();
    EXPECT_TRUE(jsonMap.contains(cookie.metadata));
    EXPECT_STREQ(jsonMap[cookie.metadata].toString().toStdString().c_str(), cookie.data.toStdString().c_str());
}
