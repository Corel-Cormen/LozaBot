#pragma once

#include <gmock/gmock.h>

#include "QEventLoopWrapper.hpp"

class MockQEventLoopWrapper : public QEventLoopWrapper
{
public:
    MOCK_METHOD(int, exec, (QEventLoop::ProcessEventsFlags flags), (override));
    MOCK_METHOD(void, quit, (), (override));
};
