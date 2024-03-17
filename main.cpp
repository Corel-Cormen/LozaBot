#include <QCoreApplication>
#include <fmt/core.h>
#include "RequestDriver.hpp"

auto& getRequestDriver()
{
    static RequestDriver drv;
    return drv;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    fmt::print("Goodbye World\n");

    RequestDriverInterface& drv = getRequestDriver();
    drv.GET();

    return a.exec();
}
