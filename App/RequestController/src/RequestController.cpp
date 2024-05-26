#include <QNetworkRequest>
#include <fmt/core.h>

#include "RequestController.hpp"
#include "JsonParser.hpp"

RequestController::RequestController(RequestDriverInterface& _requestDrv) :
    requestDrv(_requestDrv)
{

}

Error_Code_T RequestController::enterWebsite(const QString& url)
{
    Error_Code_T reqStatus = Error_Code_T::ERROR;
    QByteArray jsonData = JsonParser::parseJson(cookieCache.getCookies());
    QNetworkRequest request = JsonParser::parseRequest(url, jsonData);

    RequestDriverInterface::RequestStatus reqCode = requestDrv.GET(request, jsonData);

    if(reqCode == RequestDriverInterface::RequestStatus::OK)
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
            reqStatus = Error_Code_T::SUCCESS;
        }
        else
        {
            fmt::println("Response headers is empty");
            reqStatus = Error_Code_T::ZELOLENGTH;
        }
    }
    else
    {
        fmt::println("Request error code: {}", static_cast<int>(reqCode));
    }

    return reqStatus;
}
