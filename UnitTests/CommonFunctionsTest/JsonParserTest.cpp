#include <gtest/gtest.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>

#include "JsonParser.hpp"

class JsonParserTest : public ::testing::Test
{

};

TEST_F(JsonParserTest, createEmptyRequestTest)
{
    QNetworkRequest request = JsonParser::parseRequest("", "");

    EXPECT_STREQ(request.url().toDisplayString().toStdString().c_str(), "");
    EXPECT_STREQ(request.header(QNetworkRequest::ContentTypeHeader).toString().toStdString().c_str(), "*/*");
}

TEST_F(JsonParserTest, createRequestTest)
{
    QString url = "tmp/url";
    QByteArray jsonData = "\n\t{\n\"tmp\": \"jsonData\"\n}\n";

    QNetworkRequest request = JsonParser::parseRequest(url, jsonData);

    EXPECT_STREQ(request.url().toDisplayString().toStdString().c_str(), url.toStdString().c_str());
    EXPECT_STREQ(request.header(QNetworkRequest::ContentTypeHeader).toString().toStdString().c_str(), "*/*");
}
