#include "udpmulticastdecoder.h"

#include <QDebug>

UDPMulticastDecoder::UDPMulticastDecoder(QObject *parent)
    : ADecoder(parent)
{

}

void UDPMulticastDecoder::config(QString pIp, short pPort)
{
    mIp = pIp;
    mPort = pPort;
}

QString UDPMulticastDecoder::createGSTPipeline()
{

    QString piplineStr = QString(" udpsrc multicast-group = %1 port = %2 "
                                 " caps=\"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" "
                                 "! rtph264depay ! h264parse ! decodebin ! videoconvert ! video/x-raw,format=RGB ! queue"
                                 "! appsink name = %3 sync=false drop=true max-buffers=1 enable-last-sample=false").arg(mIp, QString::number(mPort), GST_APP_SINK_STRING);

    return piplineStr;
}
