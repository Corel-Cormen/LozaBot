#pragma once

#include <QObject>

#include "CommonTypes.hpp"

template<typename T> class QList;
template<typename T1, typename T2> class QPair;
class QByteArray;
class QNetworkRequest;

struct RequestDriverFaults
{
    bool configureRequestError;
    bool executionRequestError;
    int networkError;
};

class RequestDriverInterface : public QObject
{
    Q_OBJECT

public:
    RequestDriverInterface() = default;

    virtual ~RequestDriverInterface() = default;

    enum class RequestStatus;
    [[nodiscard]] virtual RequestStatus GET(const QNetworkRequest& request) = 0;

    typedef QList<QPair<QByteArray, QByteArray>> MetadataList;
    virtual Error_Code_T getResponseHeader(const MetadataList*& header) = 0;

    inline const RequestDriverFaults& getFault() const { return drvFaults; }

protected:
    RequestDriverFaults drvFaults{};
};


enum class RequestDriverInterface::RequestStatus
{
    REQUEST_STATUS_ERROR = -1,

    CONTINUE = 100,
    SWITCHING_PTOYOCOLS = 101,
    PROCESSING = 102,
    EARLY_HINTS = 103,

    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NON_AUTHORITAVE_INFORMATION = 203,
    NO_CONTENT = 204,
    RESET_CONTENT = 205,
    PARTIAL_CONTENT = 206,
    MULTI_STATUS = 207,
    ALREADY_REPORTED = 208,
    IM_USED = 226,

    MULTIPLE_CHOISES = 300,
    MOVED_PERMNENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    USE_PROXY = 305,
    UNUSED = 306,
    TEMPORARY_REDIRECT = 307,
    PERNAMENT_REDIRECT = 308,

    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    PAYMENT_REQUIRED = 402,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    NOT_ACCEPTABLE = 406,
    PROXY_AUTHENTICATION_REQUIRED = 407,
    REQUEST_TIMEOUT = 408,
    CONFILICT = 409,
    GONE = 410,
    LENGTH_REQUIRED = 411,
    PRECONDITIONAL_FAILED = 412,
    PAYLOAD_TOO_LARGE = 413,
    URI_TOO_LONG = 414,
    UNSUPPORTED_MEDIA_TYPE = 415,
    RANGE_NOT_SATISFIABLE = 416,
    EXPACTATION_FAILED = 417,
    IM_A_TEAPOT = 418,
    MISDIRECTED_REQUEST = 421,
    UNPROCESSABLE_CONTENT = 422,
    LOCKED = 423,
    FAILED_DEPENDANCY = 424,
    TOO_EARLY = 425,
    UPGRADE_REQUIRED = 426,
    PRECONDITION_REQURED = 428,
    TOO_MANY_REQUESTS = 429,
    REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    UNAVAILABLE_FOR_LEGAL_REASONS = 451,

    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GETEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505,
    VARIANT_ALSO_NEGOTIATES = 506,
    INSIFFICIANT_STORAGE = 507,
    LOOP_DETECTED = 508,
    NOT_EXTENDED = 510,
    NETWORK_AUTHENTICATION_REQUIRED = 511,
};
