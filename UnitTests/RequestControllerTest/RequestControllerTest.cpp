#include <gtest/gtest.h>

#include "RequestController.hpp"
#include "MockRequestDriver.hpp"

using ::testing::Return;
using ::testing::DoAll;
using ::testing::SetArgReferee;

class RequestControllerTest : public ::testing::Test
{
public:
    RequestControllerTest() :
        requestController(requestDrv)
    {

    }

protected:
    MockRequestDriver requestDrv;
    RequestController requestController;
};

TEST_F(RequestControllerTest, enterStartWebsiteRequestGetError)
{
    EXPECT_CALL(requestDrv, GET).WillOnce(Return(Error_Code_T::ERROR));

    EXPECT_EQ(Error_Code_T::ERROR, requestController.enterStartWebsite());
}

TEST_F(RequestControllerTest, enterStartWebsiteResponseHeaderIsEmpty)
{
    EXPECT_CALL(requestDrv, GET).WillOnce(Return(Error_Code_T::SUCCESS));
    EXPECT_CALL(requestDrv, getResponseHeader).WillOnce(Return(false));

    EXPECT_EQ(Error_Code_T::ZELOLENGTH, requestController.enterStartWebsite());
}

TEST_F(RequestControllerTest, enterStartWebsiteResponseCookieParseSuccesfully)
{
    RequestDriverInterface::MetadataList headersList;
    headersList.append(QPair{"metadata", "data"});
    headersList.append(QPair{"Set-Cookie", "cookieMetadata=cookieData"});

    EXPECT_CALL(requestDrv, GET).WillOnce(Return(Error_Code_T::SUCCESS));
    EXPECT_CALL(requestDrv, getResponseHeader)
            .WillOnce(DoAll(SetArgReferee<0>(headersList), Return(true)));

    EXPECT_EQ(Error_Code_T::SUCCESS, requestController.enterStartWebsite());
}
