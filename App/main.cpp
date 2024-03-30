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

        RequestDriverInterface::MetadataList headersList;
        bool resultHeader = drv.getResponseHeader(headersList);
        if(resultHeader)
        {
            foreach(auto header, headersList)
            {
                fmt::println(fmt::format("{} : {}", header.first.toStdString(), header.second.toStdString()));
            }
        }
        else
        {
            fmt::println("Response headers is empty");
        }
    }
    else
    {
        fmt::println("Request error code: {}", static_cast<int>(reqStatus));
    }

    return a.exec();
}
