#pragma once

#include <QEventLoop>
#include <QObject>

class QEventLoopWrapper : public QObject
{
    Q_OBJECT

public:
    explicit QEventLoopWrapper() = default;

    ~QEventLoopWrapper() = default;

    virtual int exec(QEventLoop::ProcessEventsFlags flags = QEventLoop::AllEvents);

public Q_SLOTS:
    virtual void quit();

private:
    QEventLoop eventLoop;
};
