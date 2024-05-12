#include <QUrl>
#include <fmt/core.h>

#include "RequestController.hpp"

RequestController::RequestController(RequestDriverInterface& _requestDrv) :
    requestDrv(_requestDrv)
{

}

Error_Code_T RequestController::enterStartWebsite()
{
    QUrl url{"https://www.zalando-lounge.pl/"};
    Error_Code_T reqStatus = requestDrv.GET(url);

    if(reqStatus == Error_Code_T::SUCCESS)
    {
        fmt::println("enter website GET request send and parse success");

        const RequestDriverInterface::MetadataList* headersList{nullptr};
        Error_Code_T resultHeader = requestDrv.getResponseHeader(headersList);
        if((resultHeader == Error_Code_T::SUCCESS) && (headersList != nullptr))
        {
            foreach(const auto& header, *headersList)
            {
                if(header.first == "Set-Cookie")
                {
                    cookieCache.updateCookies(header.second);
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
