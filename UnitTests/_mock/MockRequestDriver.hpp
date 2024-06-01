#pragma once

#include <gmock/gmock.h>

#include "RequestDriverInterface.hpp"

class MockRequestDriver : public RequestDriverInterface
{
public:
    MOCK_METHOD(RequestStatus, GET, (const QNetworkRequest& request), (override));
    MOCK_METHOD(Error_Code_T, getResponseHeader, (const MetadataList*& header), (override));
};
