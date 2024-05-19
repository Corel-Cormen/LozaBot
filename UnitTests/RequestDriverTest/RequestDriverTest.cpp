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
    QByteArray requestData{};
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

TEST_F(RequestDriverTest, ConfigureRequestNoConnect)
{
    EXPECT_CALL(mockNetworkManager, get).WillOnce(Return(nullptr));

    EXPECT_EQ(Error_Code_T::ERROR, driver.GET(request, requestData));
}

TEST_F(RequestDriverTest, ExecuteRequestError)
{
    QNetworkReplyBridge reply;

    EXPECT_CALL(mockNetworkManager, get).WillOnce(Return(&reply));
    EXPECT_CALL(mockEventLoop, exec).WillOnce(Return(-1));

    EXPECT_EQ(Error_Code_T::ERROR, driver.GET(request, requestData));
}

TEST_F(RequestDriverTest, NetworkReplyError)
{
    QNetworkReplyBridge reply;
    reply.setError(QNetworkReply::TimeoutError, "QNetworkReply::TimeoutError");

    EXPECT_CALL(mockNetworkManager, get).WillOnce(Return(&reply));
    EXPECT_CALL(mockEventLoop, exec).WillOnce(Return(0));

    EXPECT_EQ(Error_Code_T::ERROR, driver.GET(request, requestData));
}

TEST_F(RequestDriverTest, NetworkReplyBadHttpCode)
{
    QNetworkReplyBridge reply;
    reply.setError(QNetworkReply::NoError, "QNetworkReply::NoError");
    reply.setAttribute(QNetworkRequest::HttpStatusCodeAttribute, 500);

    EXPECT_CALL(mockNetworkManager, get).WillOnce(Return(&reply));
    EXPECT_CALL(mockEventLoop, exec).WillOnce(Return(0));

    EXPECT_EQ(Error_Code_T::ERROR, driver.GET(request, requestData));
}

TEST_F(RequestDriverTest, GETRequestSuccessWithEmptyResponse)
{
    QNetworkReplyBridge reply;
    reply.setError(QNetworkReply::NoError, "QNetworkReply::NoError");
    reply.setAttribute(QNetworkRequest::HttpStatusCodeAttribute, 200);

    EXPECT_CALL(mockNetworkManager, get).WillOnce(Return(&reply));
    EXPECT_CALL(mockEventLoop, exec).WillOnce(Return(0));

    ASSERT_EQ(Error_Code_T::SUCCESS, driver.GET(request, requestData));

    const RequestDriver::MetadataList* header;
    EXPECT_EQ(Error_Code_T::ZELOLENGTH, driver.getResponseHeader(header));
}

TEST_F(RequestDriverTest, GETRequestSuccessWithResponse)
{
    QNetworkReplyBridge reply;
    reply.setError(QNetworkReply::NoError, "QNetworkReply::NoError");
    reply.setAttribute(QNetworkRequest::HttpStatusCodeAttribute, 200);
    QByteArray metadataHeader = "HeaderField";
    QByteArray dataHeader = "HeaderFieldValue";
    reply.setRawHeader(metadataHeader, dataHeader);

    EXPECT_CALL(mockNetworkManager, get).WillOnce(Return(&reply));
    EXPECT_CALL(mockEventLoop, exec).WillOnce(Return(0));

    ASSERT_EQ(Error_Code_T::SUCCESS, driver.GET(request, requestData));

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
    reply.setAttribute(QNetworkRequest::HttpStatusCodeAttribute, 200);
    reply.setRawHeader("HeaderField", "HeaderFieldValue");

    EXPECT_CALL(mockNetworkManager, get).WillOnce(Return(&reply));
    EXPECT_CALL(mockEventLoop, exec).WillOnce(Return(0));

    EXPECT_EQ(Error_Code_T::SUCCESS, driver.GET(request, requestData));

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
