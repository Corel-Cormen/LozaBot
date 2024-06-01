#include <QtNetwork>
#include <fmt/core.h>

#include "RequestDriver.hpp"

RequestDriver::RequestDriver(QEventLoopWrapper& _eventLoop,
                             QNetworkAccessManagerWrapper& _networkManager) :
    networkManager(_networkManager),
    eventLoop(_eventLoop)
{

}

RequestDriver::RequestStatus RequestDriver::GET(const QNetworkRequest& request)
{
    if(configureGET(request) == Error_Code_T::SUCCESS)
    {
        if(int reqExecResult = eventLoop.exec();
                reqExecResult == 0)
        {
            fmt::println("Execute GET request");
            return receiveResponse();
        }
        else
        {
            fmt::println("Execute GET request with error: {}", reqExecResult);
            drvFaults.executionRequestError = true;
        }
    }
    else
    {
        fmt::println("Configure GET request error");
        drvFaults.configureRequestError = true;
    }

    return RequestStatus::REQUEST_STATUS_ERROR;
}

Error_Code_T RequestDriver::configureGET(const QNetworkRequest& request)
{
    networkReply = networkManager.get(request);

    bool connectResult = connect(networkReply, &QNetworkReply::finished, &eventLoop, &QEventLoopWrapper::quit);

    if(connectResult)
    {
        fmt::println("request configured to: {}", request.url().toDisplayString().toStdString());
        return Error_Code_T::SUCCESS;
    }
    else
    {
        fmt::println("request error configured to: {}", request.url().toDisplayString().toStdString());
        return Error_Code_T::ERROR;
    }
}

RequestDriver::RequestStatus RequestDriver::receiveResponse()
{
    RequestStatus requestCode = RequestStatus::REQUEST_STATUS_ERROR;;

    if(networkReply->error() == QNetworkReply::NoError)
    {
        int httpCode = networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        requestCode = convertRequestCode(httpCode);
    }
    else
    {
        fmt::println("network reply error code: {}", static_cast<int>(networkReply->error()));
        fmt::println("network reply error description: {}", networkReply->errorString().toStdString());
        drvFaults.networkError = static_cast<int>(networkReply->error());
    }

    networkReply->deleteLater();

    return requestCode;
}

Error_Code_T RequestDriver::getResponseHeader(const MetadataList*& header)
{
    if(networkReply == nullptr)
    {
        return Error_Code_T::NULLPTR;
    }

    header = &networkReply->rawHeaderPairs();

    if(header->length())
    {
        return Error_Code_T::SUCCESS;
    }
    else
    {
        return Error_Code_T::ZELOLENGTH;
    }
}

RequestDriver::RequestStatus RequestDriver::convertRequestCode(int requestCode)
{
    switch(requestCode)
    {
    case 100:
        return RequestDriver::RequestStatus::CONTINUE;
    case 101:
        return RequestDriver::RequestStatus::SWITCHING_PTOYOCOLS;
    case 102:
        return RequestDriver::RequestStatus::PROCESSING;
    case 103:
        return RequestDriver::RequestStatus::EARLY_HINTS;
    case 200:
        return RequestDriver::RequestStatus::OK;
    case 201:
        return RequestDriver::RequestStatus::CREATED;
    case 202:
        return RequestDriver::RequestStatus::ACCEPTED;
    case 203:
        return RequestDriver::RequestStatus::NON_AUTHORITAVE_INFORMATION;
    case 204:
        return RequestDriver::RequestStatus::NO_CONTENT;
    case 205:
        return RequestDriver::RequestStatus::RESET_CONTENT;
    case 206:
        return RequestDriver::RequestStatus::PARTIAL_CONTENT;
    case 207:
        return RequestDriver::RequestStatus::MULTI_STATUS;
    case 208:
        return RequestDriver::RequestStatus::ALREADY_REPORTED;
    case 226:
        return RequestDriver::RequestStatus::IM_USED;
    case 300:
        return RequestDriver::RequestStatus::MULTIPLE_CHOISES;
    case 301:
        return RequestDriver::RequestStatus::MOVED_PERMNENTLY;
    case 302:
        return RequestDriver::RequestStatus::FOUND;
    case 303:
        return RequestDriver::RequestStatus::SEE_OTHER;
    case 304:
        return RequestDriver::RequestStatus::NOT_MODIFIED;
    case 305:
        return RequestDriver::RequestStatus::USE_PROXY;
    case 306:
        return RequestDriver::RequestStatus::UNUSED;
    case 307:
        return RequestDriver::RequestStatus::TEMPORARY_REDIRECT;
    case 308:
        return RequestDriver::RequestStatus::PERNAMENT_REDIRECT;
    case 400:
        return RequestDriver::RequestStatus::BAD_REQUEST;
    case 401:
        return RequestDriver::RequestStatus::UNAUTHORIZED;
    case 402:
        return RequestDriver::RequestStatus::PAYMENT_REQUIRED;
    case 403:
        return RequestDriver::RequestStatus::FORBIDDEN;
    case 404:
        return RequestDriver::RequestStatus::NOT_FOUND;
    case 405:
        return RequestDriver::RequestStatus::METHOD_NOT_ALLOWED;
    case 406:
        return RequestDriver::RequestStatus::NOT_ACCEPTABLE;
    case 407:
        return RequestDriver::RequestStatus::PROXY_AUTHENTICATION_REQUIRED;
    case 408:
        return RequestDriver::RequestStatus::REQUEST_TIMEOUT;
    case 409:
        return RequestDriver::RequestStatus::CONFILICT;
    case 410:
        return RequestDriver::RequestStatus::GONE;
    case 411:
        return RequestDriver::RequestStatus::LENGTH_REQUIRED;
    case 412:
        return RequestDriver::RequestStatus::PRECONDITIONAL_FAILED;
    case 413:
        return RequestDriver::RequestStatus::PAYLOAD_TOO_LARGE;
    case 414:
        return RequestDriver::RequestStatus::URI_TOO_LONG;
    case 415:
        return RequestDriver::RequestStatus::UNSUPPORTED_MEDIA_TYPE;
    case 416:
        return RequestDriver::RequestStatus::RANGE_NOT_SATISFIABLE;
    case 417:
        return RequestDriver::RequestStatus::EXPACTATION_FAILED;
    case 418:
        return RequestDriver::RequestStatus::IM_A_TEAPOT;
    case 421:
        return RequestDriver::RequestStatus::MISDIRECTED_REQUEST;
    case 422:
        return RequestDriver::RequestStatus::UNPROCESSABLE_CONTENT;
    case 423:
        return RequestDriver::RequestStatus::LOCKED;
    case 424:
        return RequestDriver::RequestStatus::FAILED_DEPENDANCY;
    case 425:
        return RequestDriver::RequestStatus::TOO_EARLY;
    case 426:
        return RequestDriver::RequestStatus::UPGRADE_REQUIRED;
    case 428:
        return RequestDriver::RequestStatus::PRECONDITION_REQURED;
    case 429:
        return RequestDriver::RequestStatus::TOO_MANY_REQUESTS;
    case 431:
        return RequestDriver::RequestStatus::REQUEST_HEADER_FIELDS_TOO_LARGE;
    case 451:
        return RequestDriver::RequestStatus::UNAVAILABLE_FOR_LEGAL_REASONS;
    case 500:
        return RequestDriver::RequestStatus::INTERNAL_SERVER_ERROR;
    case 501:
        return RequestDriver::RequestStatus::NOT_IMPLEMENTED;
    case 502:
        return RequestDriver::RequestStatus::BAD_GETEWAY;
    case 503:
        return RequestDriver::RequestStatus::SERVICE_UNAVAILABLE;
    case 504:
        return RequestDriver::RequestStatus::GATEWAY_TIMEOUT;
    case 505:
        return RequestDriver::RequestStatus::HTTP_VERSION_NOT_SUPPORTED;
    case 506:
        return RequestDriver::RequestStatus::VARIANT_ALSO_NEGOTIATES;
    case 507:
        return RequestDriver::RequestStatus::INSIFFICIANT_STORAGE;
    case 508:
        return RequestDriver::RequestStatus::LOOP_DETECTED;
    case 510:
        return RequestDriver::RequestStatus::NOT_EXTENDED;
    case 511:
        return RequestDriver::RequestStatus::NETWORK_AUTHENTICATION_REQUIRED;
    default:
        return RequestDriver::RequestStatus::REQUEST_STATUS_ERROR;
    }
}
