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
