#include "udpmulticastencoder.h"
#include <QDebug>

UDPMulticastEncoder::UDPMulticastEncoder(QObject *parent)
    : AEncoder(parent)
    , mIp(QString())
    , mPort(0)
    , mWidth(0)
    , mHeight(0)
    , mFps(0)
{

}

void UDPMulticastEncoder::config(QString pIp, short pPort, short pWidth, short pHeight, short pFps)
{
    mIp = pIp;
    mPort = pPort;
    mWidth = pWidth;
    mHeight = pHeight;
    mFps = pFps;
}

QString UDPMulticastEncoder::createGSTPipeline()
{
    if(mIp.isEmpty()){
        qDebug() << "UDPMulticastStream::createGSTPipeline mIp is empty";
        return QString();
    }  

    QString pipeLine = QString("appsrc stream-type=0 is-live=true format=3 name=%1 do-timestamp=true caps=\"video/x-raw,format=BGRA"
                               ",width=%2,height=%3, pixel-aspect-ratio= 1/1, framerate= %4/1\" ! videoconvert "
                               "! x264enc speed-preset=ultrafast tune=zerolatency byte-stream=true threads=1 key-int-max=15 intra-refresh=true "
                               "! rtph264pay pt=96  ! queue ! udpsink host=%5 port=%6 auto-multicast=true sync=false").arg(GST_APP_SOURCE_STRING, QString::number(mWidth), QString::number(mHeight)
                                                                                                                           , QString::number(mFps), mIp, QString::number(mPort));

    return pipeLine;
}
