#include <QtNetwork>

#include "RequestDriver.hpp"

RequestDriver::RequestDriver() {}

void RequestDriver::receiveResponse()
{
    if(networkReply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = networkReply->readAll();
        qDebug() << "Response:";
        qDebug() << responseData;

        qDebug() << "";

        QList<QByteArray> headers = networkReply->rawHeaderList();
        qDebug() << "Headers:";
        foreach(const QByteArray& header, headers)
        {
            qDebug() << header << ":" << networkReply->rawHeader(header);
        }
    }
    else
    {
        qDebug() << "Error status code:" << networkReply->error();
    }

    networkReply->deleteLater();
}

void RequestDriver::reciveCall()
{
    connect(networkReply, &QNetworkReply::finished, this, &RequestDriver::receiveResponse);
}

void RequestDriver::GET()
{
    QNetworkAccessManager manager;

    QNetworkRequest request(QUrl("https://www.zalando-lounge.pl"));
    networkReply = manager.get(request);

    // TO DO: QEventLoop implement to get rid timer

    reciveCall();

    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}
