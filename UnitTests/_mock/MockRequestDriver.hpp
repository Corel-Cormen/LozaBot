#pragma once

#include <gmock/gmock.h>

#include "RequestDriverInterface.hpp"

class MockRequestDriver : public RequestDriverInterface
{
public:
    MOCK_METHOD(Error_Code_T, GET, (const QUrl& url), (override));
    MOCK_METHOD(bool, getResponseHeader, (MetadataList& header), (override));
};
