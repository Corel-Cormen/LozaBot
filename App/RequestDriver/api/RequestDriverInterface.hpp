#pragma once

#include <QObject>

#include "CommonTypes.hpp"

class RequestDriverInterface : public QObject
{
    Q_OBJECT

public:

    RequestDriverInterface() = default;

    virtual ~RequestDriverInterface() = default;

    [[nodiscard]] virtual Error_Code_T GET(const QUrl& url) = 0;

    typedef QList<QPair<QString, QString>> MetadataList;
    virtual bool getResponseHeader(MetadataList& header) = 0;
};
