#include "QEventLoopWrapper.hpp"

int QEventLoopWrapper::exec(QEventLoop::ProcessEventsFlags flags)
{
    return eventLoop.exec(flags);
}

void QEventLoopWrapper::quit()
{
    eventLoop.quit();
}
