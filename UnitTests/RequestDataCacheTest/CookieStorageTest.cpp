#include <gtest/gtest.h>

#include "CookieStorage.hpp"

class CookieStorageSeachTest : public ::testing::TestWithParam<std::pair<CookieData, int>>
{
public:
    CookieStorageSeachTest()
    {
        foreach(const auto& cookie, patternSearch)
        {
            cookieStorage.update(cookie);
        }
    }

protected:
    CookieStorage cookieStorage;
    QList<CookieData> patternSearch = {
        CookieData{ .metadata = "metadata_0" },
        CookieData{ .metadata = "metadata_1" },
        CookieData{ .metadata = "metadata_2" },
    };
};

TEST_P(CookieStorageSeachTest, searchTest)
{
    CookieData searchData = GetParam().first;
    int result = GetParam().second;

    ASSERT_EQ(cookieStorage.getSize(), patternSearch.length());
    EXPECT_EQ(cookieStorage.indexOf(searchData), result);
}

INSTANTIATE_TEST_SUITE_P(CookieStorageSeachParamTest, CookieStorageSeachTest,
    ::testing::Values(
        std::make_pair(CookieData{ .metadata = "metadata_0"}, 0),
        std::make_pair(CookieData{ .metadata = "metadata_1"}, 1),
        std::make_pair(CookieData{ .metadata = "metadata_2"}, 2),
        std::make_pair(CookieData{ .metadata = "no_metadata"}, -1),
        std::make_pair(CookieData{ .metadata = ""}, -1)
    )
);

class CookieStorageTest : public ::testing::Test
{

};

TEST_F(CookieStorageTest, getSizeTest)
{
    CookieStorage storage;
    EXPECT_EQ(storage.getSize(), 0);

    storage.update(CookieData{});
    EXPECT_EQ(storage.getSize(), 1);
}

TEST_F(CookieStorageTest, getCookieTest)
{
    CookieData patternData0 = CookieData{.metadata = "metadata0", .data = "data0"};
    CookieData patternData1 = CookieData{.metadata = "metadata1", .data = "data1"};

    CookieStorage storage;
    storage.update(patternData0);
    storage.update(patternData1);
    ASSERT_EQ(storage.getSize(), 2);

    EXPECT_EQ(storage.getCookie(0).metadata, patternData0.metadata);
    EXPECT_EQ(storage.getCookie(0).data, patternData0.data);
    EXPECT_EQ(storage.getCookie(1).metadata, patternData1.metadata);
    EXPECT_EQ(storage.getCookie(1).data, patternData1.data);
}

TEST_F(CookieStorageTest, updateCookieAddTwoElementTest)
{
    CookieData patternData0 = CookieData{.metadata = "metadata0", .data = "data0"};
    CookieData patternData1 = CookieData{.metadata = "metadata1", .data = "data1"};

    CookieStorage storage;
    storage.update(patternData0);
    EXPECT_EQ(storage.getSize(), 1);
    storage.update(patternData1);
    EXPECT_EQ(storage.getSize(), 2);
}

TEST_F(CookieStorageTest, updateCookieAddAndUpdateTest)
{
    CookieData patternData = CookieData{.metadata = "metadata0", .data = "data0", .expires = 0};

    CookieStorage storage;
    storage.update(patternData);
    ASSERT_EQ(storage.getSize(), 1);
    EXPECT_EQ(storage.getCookie(0).metadata, patternData.metadata);
    EXPECT_EQ(storage.getCookie(0).data, patternData.data);
    EXPECT_EQ(storage.getCookie(0).expires, patternData.expires);

    patternData.data = "data1";
    patternData.expires = 10;

    storage.update(patternData);
    ASSERT_EQ(storage.getSize(), 1);
    EXPECT_EQ(storage.getCookie(0).metadata, patternData.metadata);
    EXPECT_EQ(storage.getCookie(0).data, patternData.data);
    EXPECT_EQ(storage.getCookie(0).expires, patternData.expires);
}

TEST_F(CookieStorageTest, updateCookieAddAndNoUpdateTest)
{
    CookieData patternData = CookieData{.metadata = "metadata0", .data = "data0", .expires = 0};

    CookieStorage storage;
    storage.update(patternData);
    ASSERT_EQ(storage.getSize(), 1);
    EXPECT_EQ(storage.getCookie(0).metadata, patternData.metadata);
    EXPECT_EQ(storage.getCookie(0).data, patternData.data);
    EXPECT_EQ(storage.getCookie(0).expires, patternData.expires);

    patternData.expires = 10;

    storage.update(patternData);
    ASSERT_EQ(storage.getSize(), 1);
    EXPECT_EQ(storage.getCookie(0).metadata, patternData.metadata);
    EXPECT_EQ(storage.getCookie(0).data, patternData.data);
    EXPECT_NE(storage.getCookie(0).expires, patternData.expires);
}
