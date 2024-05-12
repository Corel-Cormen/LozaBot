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
   const RequestDriverInterface::MetadataList headersList;
   EXPECT_CALL(requestDrv, getResponseHeader).WillOnce(
               DoAll(SetArgReferee<0>(&headersList), Return(Error_Code_T::ZELOLENGTH)));

   EXPECT_EQ(Error_Code_T::ZELOLENGTH, requestController.enterStartWebsite());
}

TEST_F(RequestControllerTest, enterStartWebsiteResponseHeaderIsNullptr)
{
   EXPECT_CALL(requestDrv, GET).WillOnce(Return(Error_Code_T::SUCCESS));
   EXPECT_CALL(requestDrv, getResponseHeader).WillOnce(
               DoAll(SetArgReferee<0>(nullptr), Return(Error_Code_T::SUCCESS)));

   EXPECT_EQ(Error_Code_T::ZELOLENGTH, requestController.enterStartWebsite());
}

TEST_F(RequestControllerTest, enterStartWebsiteResponseHeaderIsNullptrAndResultIsError)
{
   EXPECT_CALL(requestDrv, GET).WillOnce(Return(Error_Code_T::SUCCESS));
   EXPECT_CALL(requestDrv, getResponseHeader).WillOnce(
               DoAll(SetArgReferee<0>(nullptr), Return(Error_Code_T::ERROR)));

   EXPECT_EQ(Error_Code_T::ZELOLENGTH, requestController.enterStartWebsite());
}

TEST_F(RequestControllerTest, enterStartWebsiteResponseCookieParseSuccesfully)
{
   const RequestDriverInterface::MetadataList headersList{
       {"metadata", "data"},
       {"Set-Cookie", "cookieMetadata=cookieData"}
   };

   EXPECT_CALL(requestDrv, GET).WillOnce(Return(Error_Code_T::SUCCESS));
   EXPECT_CALL(requestDrv, getResponseHeader)
           .WillOnce(DoAll(SetArgReferee<0>(&headersList), Return(Error_Code_T::SUCCESS)));

   EXPECT_EQ(Error_Code_T::SUCCESS, requestController.enterStartWebsite());
}
