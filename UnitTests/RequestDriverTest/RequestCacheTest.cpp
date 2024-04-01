#include <gtest/gtest.h>

#include "RequestCache.hpp"

class RequestCacheTest : public ::testing::Test
{
protected:
    RequestCache cache;
};

TEST_F(RequestCacheTest, PutAndGetHeader)
{
    const QString data = "data";
    const QString metadata = "metadata";
    cache.putHeader(data, metadata);

    auto header = cache.getHeaders();
    EXPECT_TRUE(header.has_value());
    ASSERT_EQ(1, header.value().length());
    EXPECT_STREQ(data.toStdString().c_str(), header.value().at(0).first.toStdString().c_str());
    EXPECT_STREQ(metadata.toStdString().c_str(), header.value().at(0).second.toStdString().c_str());
}

TEST_F(RequestCacheTest, GetEmptyHeader)
{
    auto header = cache.getHeaders();
    EXPECT_FALSE(header.has_value());
}

TEST_F(RequestCacheTest, PutAndGetHeaderNextCheckEmptyGet)
{
    const QString data = "data";
    const QString metadata = "metadata";
    cache.putHeader(data, metadata);

    auto header = cache.getHeaders();
    header = cache.getHeaders();
    EXPECT_FALSE(header.has_value());
}
