#include <QCoreApplication>
#include <fmt/core.h>
#include "RequestDriver.hpp"
#include "QEventLoopWrapper.hpp"
#include "QNetworkAccessManagerWrapper.hpp"
#include "RequestController.hpp"

auto& getRequestDriver()
{
    static QEventLoopWrapper eventLoop;
    static QNetworkAccessManagerWrapper networkManager;
    static RequestDriver drv{eventLoop, networkManager};
    return drv;
}

auto& getRequestController()
{
    static RequestController requestController{getRequestDriver()};
    return requestController;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    RequestControllerInterface& requestController = getRequestController();

    requestController.enterStartWebsite();

    return a.exec();
}
