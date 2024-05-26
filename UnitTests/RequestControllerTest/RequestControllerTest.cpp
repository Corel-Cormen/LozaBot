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

    QString url{"tmp/url/website"};
};

TEST_F(RequestControllerTest, enterStartWebsiteRequestGetError)
{
    EXPECT_CALL(requestDrv, GET).WillOnce(Return(RequestDriverInterface::RequestStatus::REQUEST_STATUS_ERROR));
    EXPECT_EQ(Error_Code_T::ERROR, requestController.enterWebsite(url));

    EXPECT_CALL(requestDrv, GET).WillOnce(Return(RequestDriverInterface::RequestStatus::BAD_REQUEST));
    EXPECT_EQ(Error_Code_T::ERROR, requestController.enterWebsite(url));
}

TEST_F(RequestControllerTest, enterStartWebsiteResponseHeaderIsEmpty)
{
   EXPECT_CALL(requestDrv, GET).WillOnce(Return(RequestDriverInterface::RequestStatus::OK));
   const RequestDriverInterface::MetadataList headersList;
   EXPECT_CALL(requestDrv, getResponseHeader).WillOnce(
               DoAll(SetArgReferee<0>(&headersList), Return(Error_Code_T::ZELOLENGTH)));

   EXPECT_EQ(Error_Code_T::ZELOLENGTH, requestController.enterWebsite(url));
}

TEST_F(RequestControllerTest, enterStartWebsiteResponseHeaderIsNullptr)
{
   EXPECT_CALL(requestDrv, GET).WillOnce(Return(RequestDriverInterface::RequestStatus::OK));
   EXPECT_CALL(requestDrv, getResponseHeader).WillOnce(
               DoAll(SetArgReferee<0>(nullptr), Return(Error_Code_T::SUCCESS)));

   EXPECT_EQ(Error_Code_T::ZELOLENGTH, requestController.enterWebsite(url));
}

TEST_F(RequestControllerTest, enterStartWebsiteResponseHeaderIsNullptrAndResultIsError)
{
   EXPECT_CALL(requestDrv, GET).WillOnce(Return(RequestDriverInterface::RequestStatus::OK));
   EXPECT_CALL(requestDrv, getResponseHeader).WillOnce(
               DoAll(SetArgReferee<0>(nullptr), Return(Error_Code_T::ERROR)));

   EXPECT_EQ(Error_Code_T::ZELOLENGTH, requestController.enterWebsite(url));
}

TEST_F(RequestControllerTest, enterStartWebsiteResponseCookieParseSuccesfully)
{
   const RequestDriverInterface::MetadataList headersList{
       {"metadata", "data"},
       {"Set-Cookie", "cookieMetadata=cookieData"}
   };

   EXPECT_CALL(requestDrv, GET).WillOnce(Return(RequestDriverInterface::RequestStatus::OK));
   EXPECT_CALL(requestDrv, getResponseHeader)
           .WillOnce(DoAll(SetArgReferee<0>(&headersList), Return(Error_Code_T::SUCCESS)));

   EXPECT_EQ(Error_Code_T::SUCCESS, requestController.enterWebsite(url));
}
