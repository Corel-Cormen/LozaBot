#include <gtest/gtest.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>

#include "JsonParser.hpp"

class JsonParserTest : public ::testing::Test
{

};

TEST_F(JsonParserTest, parseEmptyJsonTest)
{
    QJsonObject json;
    QByteArray pattern = QJsonDocument{json}.toJson();

    QByteArray jsonRaw = JsonParser::parseJson(json);
    EXPECT_STREQ(jsonRaw, pattern);
}

TEST_F(JsonParserTest, parseJsonTest)
{
    QJsonObject json;
    json["metadata"] = "data";
    QByteArray pattern = QJsonDocument{json}.toJson();

    QByteArray jsonRaw = JsonParser::parseJson(json);
    EXPECT_STREQ(jsonRaw, pattern);
}

TEST_F(JsonParserTest, createEmptyRequestTest)
{
    QNetworkRequest request = JsonParser::parseRequest("", "");

    EXPECT_STREQ(request.url().toDisplayString().toStdString().c_str(), "");
    EXPECT_STREQ(request.header(QNetworkRequest::ContentTypeHeader).toString().toStdString().c_str(), "application/json");
    EXPECT_EQ(request.header(QNetworkRequest::ContentLengthHeader).toUInt(), 0);
}

TEST_F(JsonParserTest, createRequestTest)
{
    QString url = "tmp/url";
    QByteArray jsonData = "\n\t{\n\"tmp\": \"jsonData\"\n}\n";

    QNetworkRequest request = JsonParser::parseRequest(url, jsonData);

    EXPECT_STREQ(request.url().toDisplayString().toStdString().c_str(), url.toStdString().c_str());
    EXPECT_STREQ(request.header(QNetworkRequest::ContentTypeHeader).toString().toStdString().c_str(), "application/json");
    EXPECT_EQ(request.header(QNetworkRequest::ContentLengthHeader).toUInt(), jsonData.length());
}
