#include <QCoreApplication>
#include <fmt/core.h>
#include "RequestDriver.hpp"
#include "QEventLoopWrapper.hpp"
#include "QNetworkAccessManagerWrapper.hpp"

auto& getRequestDriver()
{
    static QEventLoopWrapper eventLoop;
    static QNetworkAccessManagerWrapper networkManager;
    static RequestDriver drv{eventLoop, networkManager};
    return drv;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    RequestDriverInterface& drv = getRequestDriver();

    QUrl url{"https://www.zalando-lounge.pl/"};
    Error_Code_T reqStatus = drv.GET(url);

    if(reqStatus == Error_Code_T::SUCCESS)
    {
        fmt::println("GET request send and parse success");
    }
    else
    {
        fmt::println("Request error code: {}", static_cast<int>(reqStatus));
    }

    return a.exec();
}
