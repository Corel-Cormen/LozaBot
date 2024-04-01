#include <QUrl>
#include <fmt/core.h>

#include "RequestController.hpp"
#include "CookieCache.hpp"

RequestController::RequestController(RequestDriverInterface& _requestDrv) :
    requestDrv(_requestDrv),
    cookieCache(new CookieCache)
{

}

RequestController::~RequestController()
{
    delete cookieCache;
}

Error_Code_T RequestController::enterStartWebsite()
{
    QUrl url{"https://www.zalando-lounge.pl/"};
    Error_Code_T reqStatus = requestDrv.GET(url);

    if(reqStatus == Error_Code_T::SUCCESS)
    {
        fmt::println("enter website GET request send and parse success");

        RequestDriverInterface::MetadataList headersList;
        bool resultHeader = requestDrv.getResponseHeader(headersList);
        if(resultHeader)
        {
            foreach(auto header, headersList)
            {
                if(header.first == "Set-Cookie")
                {
                    cookieCache->parseCookies(std::move(header.second));
                }
            }
        }
        else
        {
            fmt::println("Response headers is empty");
            reqStatus = Error_Code_T::ZELOLENGTH;
        }
    }
    else
    {
        fmt::println("Request error code: {}", static_cast<int>(reqStatus));
    }

    return reqStatus;
}
