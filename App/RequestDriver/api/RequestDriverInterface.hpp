#pragma once

#include <QObject>

#include "CommonTypes.hpp"

template<typename T> class QList;
template<typename T1, typename T2> class QPair;
class QByteArray;
class QNetworkRequest;

class RequestDriverInterface : public QObject
{
    Q_OBJECT

public:

    RequestDriverInterface() = default;

    virtual ~RequestDriverInterface() = default;

    [[nodiscard]] virtual Error_Code_T GET(const QNetworkRequest& request, const QByteArray& data) = 0;

    typedef QList<QPair<QByteArray, QByteArray>> MetadataList;
    virtual Error_Code_T getResponseHeader(const MetadataList*& header) = 0;
};
