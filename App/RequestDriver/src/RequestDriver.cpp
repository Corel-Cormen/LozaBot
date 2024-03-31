#include <QtNetwork>
#include <fmt/core.h>

#include "RequestDriver.hpp"
#include "RequestCache.hpp"

RequestDriver::RequestDriver(QEventLoopWrapper& _eventLoop,
                             QNetworkAccessManagerWrapper& _networkManager) :
    networkManager(_networkManager),
    eventLoop(_eventLoop),
    requestCache(new RequestCache)
{

}

RequestDriver::~RequestDriver()
{
    delete requestCache;
}

Error_Code_T RequestDriver::GET(const QUrl& url)
{
    Error_Code_T status = configureGET(url);

    if(status == Error_Code_T::SUCCESS)
    {
        if(int reqExecResult = eventLoop.exec();
                reqExecResult == 0)
        {
            fmt::println("Execute GET request");
            status = receiveResponse();
        }
        else
        {
            fmt::println("Execute GET request with error: {}", reqExecResult);
            status = Error_Code_T::ERROR;
        }
    }
    else
    {
        fmt::println("Configure GET request error");
    }

    return status;
}

Error_Code_T RequestDriver::configureGET(const QUrl& url)
{
    networkReply = networkManager.get(QNetworkRequest(url));

    bool connectResult = connect(networkReply,
                                 &QNetworkReply::finished,
                                 &eventLoop,
                                 &QEventLoopWrapper::quit);

    if(connectResult)
    {
        fmt::println("request configured to: {}", url.toDisplayString().toStdString());
        return Error_Code_T::SUCCESS;
    }
    else
    {
        fmt::println("request error configured to: {}", url.toDisplayString().toStdString());
        return Error_Code_T::ERROR;
    }
}

Error_Code_T RequestDriver::receiveResponse()
{
    Error_Code_T status = Error_Code_T::ERROR;

    if(networkReply->error() == QNetworkReply::NoError)
    {
        if(int httpCode = networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                httpCode == 200)
        {
            status = wrapHeader();
        }
        else
        {
            fmt::println("Request response with code: {}", httpCode);
        }
    }
    else
    {
        fmt::println("network reply error code: {}", static_cast<int>(networkReply->error()));
        fmt::println("network reply error description: {}", networkReply->errorString().toStdString());
    }

    networkReply->deleteLater();

    return status;
}

Error_Code_T RequestDriver::wrapHeader()
{
    if(requestCache == nullptr)
    {
        return Error_Code_T::NULLPTR;
    }

    foreach(auto headerField, networkReply->rawHeaderPairs())
    {
        requestCache->putHeader(headerField.first, headerField.second);
    }

    return Error_Code_T::SUCCESS;
}

bool RequestDriver::getResponseHeader(MetadataList& header)
{
    auto moveHeader = requestCache->getHeaders();
    if(moveHeader.has_value())
    {
        header = moveHeader.value();
        return true;
    }
    else
    {
        header.clear();
        return false;
    }
}
