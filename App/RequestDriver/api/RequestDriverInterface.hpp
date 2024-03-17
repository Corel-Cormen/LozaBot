#pragma once

#include <QObject>

class RequestDriverInterface : public QObject
{
    Q_OBJECT

public:

    RequestDriverInterface() = default;

    virtual ~RequestDriverInterface() = default;

    virtual void GET() = 0;
};
