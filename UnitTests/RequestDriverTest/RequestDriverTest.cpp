#include <gtest/gtest.h>

#include "RequestDriver.hpp"
#include "MockQEventLoopWrapper.hpp"
#include "MockQNetworkAccessManagerWrapper.hpp"

using ::testing::Return;

class RequestDriverTest : public ::testing::Test
{
public:
    RequestDriverTest() :
        driver(mockEventLoop, mockNetworkManager)
    {

    }

protected:
    MockQEventLoopWrapper mockEventLoop;
    MockQNetworkAccessManagerWrapper mockNetworkManager;
    RequestDriver driver;

    QNetworkRequest request{};
};

class QNetworkReplyBridge : public QNetworkReply
{
public:
    virtual void abort() override;

    virtual qint64 readData(char* data, qint64 maxlen) override;
    virtual qint64 writeData(const char* data, qint64 len) override;

    void setError(NetworkError errorCode, const QString& errorString);
    void setAttribute(QNetworkRequest::Attribute code, const QVariant& value);
    void setRawHeader(const QByteArray& headerName, const QByteArray& value);
};

void QNetworkReplyBridge::abort()
{

}

qint64 QNetworkReplyBridge::readData(char* data, qint64 maxlen)
{
    return -1;
}

qint64 QNetworkReplyBridge::writeData(const char* data, qint64 len)
{
    return -1;
}

void QNetworkReplyBridge::setError(NetworkError errorCode, const QString& errorString)
{
    QNetworkReply::setError(errorCode, errorString);
}

void QNetworkReplyBridge::setAttribute(QNetworkRequest::Attribute code, const QVariant& value)
{
    QNetworkReply::setAttribute(code, value);
}

void QNetworkReplyBridge::setRawHeader(const QByteArray& headerName, const QByteArray& value)
{
    QNetworkReply::setRawHeader(headerName, value);
}

TEST_F(RequestDriverTest, ChcekDefaultFaults)
{
    const RequestDriverFaults drvFaults = driver.getFault();

    EXPECT_FALSE(drvFaults.configureRequestError);
    EXPECT_FALSE(drvFaults.executionRequestError);
    EXPECT_EQ(drvFaults.networkError, 0);
}

TEST_F(RequestDriverTest, ConfigureRequestNoConnect)
{
    EXPECT_CALL(mockNetworkManager, get).WillOnce(Return(nullptr));

    EXPECT_EQ(RequestDriver::RequestStatus::REQUEST_STATUS_ERROR, driver.GET(request));
    EXPECT_TRUE(driver.getFault().configureRequestError);
}

TEST_F(RequestDriverTest, ExecuteRequestError)
{
    QNetworkReplyBridge reply;

    EXPECT_CALL(mockNetworkManager, get).WillOnce(Return(&reply));
    EXPECT_CALL(mockEventLoop, exec).WillOnce(Return(-1));

    EXPECT_EQ(RequestDriver::RequestStatus::REQUEST_STATUS_ERROR, driver.GET(request));
    EXPECT_TRUE(driver.getFault().executionRequestError);
}

TEST_F(RequestDriverTest, NetworkReplyError)
{
    QNetworkReplyBridge reply;
    reply.setError(QNetworkReply::TimeoutError, "QNetworkReply::TimeoutError");

    EXPECT_CALL(mockNetworkManager, get).WillOnce(Return(&reply));
    EXPECT_CALL(mockEventLoop, exec).WillOnce(Return(0));

    EXPECT_EQ(RequestDriver::RequestStatus::REQUEST_STATUS_ERROR, driver.GET(request));
    EXPECT_EQ(driver.getFault().networkError, static_cast<int>(QNetworkReply::TimeoutError));
}

TEST_F(RequestDriverTest, GETRequestSuccessCheckFaults)
{
    QNetworkReplyBridge reply;
    reply.setError(QNetworkReply::NoError, "QNetworkReply::NoError");

    EXPECT_CALL(mockNetworkManager, get).WillOnce(Return(&reply));
    EXPECT_CALL(mockEventLoop, exec).WillOnce(Return(0));

    (void) driver.GET(request);
    const RequestDriverFaults drvFaults = driver.getFault();

    EXPECT_FALSE(drvFaults.configureRequestError);
    EXPECT_FALSE(drvFaults.executionRequestError);
    EXPECT_EQ(drvFaults.networkError, 0);
}

TEST_F(RequestDriverTest, GETRequestSuccessWithEmptyResponse)
{
    QNetworkReplyBridge reply;
    reply.setError(QNetworkReply::NoError, "QNetworkReply::NoError");

    EXPECT_CALL(mockNetworkManager, get).WillOnce(Return(&reply));
    EXPECT_CALL(mockEventLoop, exec).WillOnce(Return(0));

    (void) driver.GET(request);

    const RequestDriver::MetadataList* header;
    EXPECT_EQ(Error_Code_T::ZELOLENGTH, driver.getResponseHeader(header));
}

TEST_F(RequestDriverTest, GETRequestSuccessWithResponse)
{
    QNetworkReplyBridge reply;
    reply.setError(QNetworkReply::NoError, "QNetworkReply::NoError");
    QByteArray metadataHeader = "HeaderField";
    QByteArray dataHeader = "HeaderFieldValue";
    reply.setRawHeader(metadataHeader, dataHeader);

    EXPECT_CALL(mockNetworkManager, get).WillOnce(Return(&reply));
    EXPECT_CALL(mockEventLoop, exec).WillOnce(Return(0));

    (void) driver.GET(request);

    const RequestDriver::MetadataList* header;
    EXPECT_EQ(Error_Code_T::SUCCESS, driver.getResponseHeader(header));
    EXPECT_FALSE(header->isEmpty());
    ASSERT_EQ(1, header->length());
    EXPECT_EQ(metadataHeader, header->at(0).first);
    EXPECT_EQ(dataHeader, header->at(0).second);
}

TEST_F(RequestDriverTest, GETRequestSuccessNoDeleteMessageTest)
{
    QNetworkReplyBridge reply;
    reply.setError(QNetworkReply::NoError, "QNetworkReply::NoError");
    reply.setRawHeader("HeaderField", "HeaderFieldValue");

    EXPECT_CALL(mockNetworkManager, get).WillOnce(Return(&reply));
    EXPECT_CALL(mockEventLoop, exec).WillOnce(Return(0));

    (void) driver.GET(request);

    const RequestDriver::MetadataList* header;
    EXPECT_EQ(Error_Code_T::SUCCESS, driver.getResponseHeader(header));
    EXPECT_FALSE(header->isEmpty());
    ASSERT_EQ(1, header->length());

    EXPECT_EQ(Error_Code_T::SUCCESS, driver.getResponseHeader(header));
    EXPECT_FALSE(header->isEmpty());
    ASSERT_EQ(1, header->length());
}

TEST_F(RequestDriverTest, getResponseWhenRequestNotExecute)
{
    const RequestDriver::MetadataList* header;
    EXPECT_EQ(Error_Code_T::NULLPTR, driver.getResponseHeader(header));
}

class RequestDriverMapingCodeTest : public ::testing::TestWithParam<std::pair<int, RequestDriver::RequestStatus>>
{
protected:
    MockQEventLoopWrapper mockEventLoop;
    MockQNetworkAccessManagerWrapper mockNetworkManager;
    RequestDriver driver{mockEventLoop, mockNetworkManager};

    QNetworkRequest request{};
};

TEST_P(RequestDriverMapingCodeTest, codeMapTest)
{
    int returnCodeByRequest = GetParam().first;
    RequestDriver::RequestStatus expectCode = GetParam().second;

    QNetworkReplyBridge reply;
    reply.setError(QNetworkReply::NoError, "QNetworkReply::NoError");
    reply.setAttribute(QNetworkRequest::HttpStatusCodeAttribute, returnCodeByRequest);

    EXPECT_CALL(mockNetworkManager, get).WillOnce(Return(&reply));
    EXPECT_CALL(mockEventLoop, exec).WillOnce(Return(0));

    RequestDriver::RequestStatus requestCode = driver.GET(request);

    EXPECT_EQ(requestCode, expectCode);
}

INSTANTIATE_TEST_SUITE_P(RequestDriverParamTest, RequestDriverMapingCodeTest,
    ::testing::Values(
        std::make_pair(0, RequestDriver::RequestStatus::REQUEST_STATUS_ERROR),
        std::make_pair(-1, RequestDriver::RequestStatus::REQUEST_STATUS_ERROR),
        std::make_pair(50, RequestDriver::RequestStatus::REQUEST_STATUS_ERROR),
        std::make_pair(600, RequestDriver::RequestStatus::REQUEST_STATUS_ERROR),

        std::make_pair(100, RequestDriver::RequestStatus::CONTINUE),
        std::make_pair(101, RequestDriver::RequestStatus::SWITCHING_PTOYOCOLS),
        std::make_pair(102, RequestDriver::RequestStatus::PROCESSING),
        std::make_pair(103, RequestDriver::RequestStatus::EARLY_HINTS),
        std::make_pair(200, RequestDriver::RequestStatus::OK),
        std::make_pair(201, RequestDriver::RequestStatus::CREATED),
        std::make_pair(202, RequestDriver::RequestStatus::ACCEPTED),
        std::make_pair(203, RequestDriver::RequestStatus::NON_AUTHORITAVE_INFORMATION),
        std::make_pair(204, RequestDriver::RequestStatus::NO_CONTENT),
        std::make_pair(205, RequestDriver::RequestStatus::RESET_CONTENT),
        std::make_pair(206, RequestDriver::RequestStatus::PARTIAL_CONTENT),
        std::make_pair(207, RequestDriver::RequestStatus::MULTI_STATUS),
        std::make_pair(208, RequestDriver::RequestStatus::ALREADY_REPORTED),
        std::make_pair(226, RequestDriver::RequestStatus::IM_USED),
        std::make_pair(300, RequestDriver::RequestStatus::MULTIPLE_CHOISES),
        std::make_pair(301, RequestDriver::RequestStatus::MOVED_PERMNENTLY),
        std::make_pair(302, RequestDriver::RequestStatus::FOUND),
        std::make_pair(303, RequestDriver::RequestStatus::SEE_OTHER),
        std::make_pair(304, RequestDriver::RequestStatus::NOT_MODIFIED),
        std::make_pair(305, RequestDriver::RequestStatus::USE_PROXY),
        std::make_pair(306, RequestDriver::RequestStatus::UNUSED),
        std::make_pair(307, RequestDriver::RequestStatus::TEMPORARY_REDIRECT),
        std::make_pair(308, RequestDriver::RequestStatus::PERNAMENT_REDIRECT),
        std::make_pair(400, RequestDriver::RequestStatus::BAD_REQUEST),
        std::make_pair(401, RequestDriver::RequestStatus::UNAUTHORIZED),
        std::make_pair(402, RequestDriver::RequestStatus::PAYMENT_REQUIRED),
        std::make_pair(403, RequestDriver::RequestStatus::FORBIDDEN),
        std::make_pair(404, RequestDriver::RequestStatus::NOT_FOUND),
        std::make_pair(405, RequestDriver::RequestStatus::METHOD_NOT_ALLOWED),
        std::make_pair(406, RequestDriver::RequestStatus::NOT_ACCEPTABLE),
        std::make_pair(407, RequestDriver::RequestStatus::PROXY_AUTHENTICATION_REQUIRED),
        std::make_pair(408, RequestDriver::RequestStatus::REQUEST_TIMEOUT),
        std::make_pair(409, RequestDriver::RequestStatus::CONFILICT),
        std::make_pair(410, RequestDriver::RequestStatus::GONE),
        std::make_pair(411, RequestDriver::RequestStatus::LENGTH_REQUIRED),
        std::make_pair(412, RequestDriver::RequestStatus::PRECONDITIONAL_FAILED),
        std::make_pair(413, RequestDriver::RequestStatus::PAYLOAD_TOO_LARGE),
        std::make_pair(414, RequestDriver::RequestStatus::URI_TOO_LONG),
        std::make_pair(415, RequestDriver::RequestStatus::UNSUPPORTED_MEDIA_TYPE),
        std::make_pair(416, RequestDriver::RequestStatus::RANGE_NOT_SATISFIABLE),
        std::make_pair(417, RequestDriver::RequestStatus::EXPACTATION_FAILED),
        std::make_pair(418, RequestDriver::RequestStatus::IM_A_TEAPOT),
        std::make_pair(421, RequestDriver::RequestStatus::MISDIRECTED_REQUEST),
        std::make_pair(422, RequestDriver::RequestStatus::UNPROCESSABLE_CONTENT),
        std::make_pair(423, RequestDriver::RequestStatus::LOCKED),
        std::make_pair(424, RequestDriver::RequestStatus::FAILED_DEPENDANCY),
        std::make_pair(425, RequestDriver::RequestStatus::TOO_EARLY),
        std::make_pair(426, RequestDriver::RequestStatus::UPGRADE_REQUIRED),
        std::make_pair(428, RequestDriver::RequestStatus::PRECONDITION_REQURED),
        std::make_pair(429, RequestDriver::RequestStatus::TOO_MANY_REQUESTS),
        std::make_pair(431, RequestDriver::RequestStatus::REQUEST_HEADER_FIELDS_TOO_LARGE),
        std::make_pair(451, RequestDriver::RequestStatus::UNAVAILABLE_FOR_LEGAL_REASONS),
        std::make_pair(500, RequestDriver::RequestStatus::INTERNAL_SERVER_ERROR),
        std::make_pair(501, RequestDriver::RequestStatus::NOT_IMPLEMENTED),
        std::make_pair(502, RequestDriver::RequestStatus::BAD_GETEWAY),
        std::make_pair(503, RequestDriver::RequestStatus::SERVICE_UNAVAILABLE),
        std::make_pair(504, RequestDriver::RequestStatus::GATEWAY_TIMEOUT),
        std::make_pair(505, RequestDriver::RequestStatus::HTTP_VERSION_NOT_SUPPORTED),
        std::make_pair(506, RequestDriver::RequestStatus::VARIANT_ALSO_NEGOTIATES),
        std::make_pair(507, RequestDriver::RequestStatus::INSIFFICIANT_STORAGE),
        std::make_pair(508, RequestDriver::RequestStatus::LOOP_DETECTED),
        std::make_pair(510, RequestDriver::RequestStatus::NOT_EXTENDED),
        std::make_pair(511, RequestDriver::RequestStatus::NETWORK_AUTHENTICATION_REQUIRED)
    )
);
