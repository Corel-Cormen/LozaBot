#pragma once

#include <gmock/gmock.h>

#include "RequestDriverInterface.hpp"

class MockRequestDriver : public RequestDriverInterface
{
public:
    MOCK_METHOD(Error_Code_T, GET, (const QNetworkRequest& request, const QByteArray& data), (override));
    MOCK_METHOD(Error_Code_T, getResponseHeader, (const MetadataList*& header), (override));
};
