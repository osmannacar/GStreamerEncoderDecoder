#include <QApplication>
#include <QDebug>
#include <QDateTime>
#include <QPainter>
#include <QLabel>
#include <QTimer>
#include <QPushButton>

#include "encoder/udpmulticastencoder.h"
#include "encoder/udpunicastencoder.h"
#include "decoder/udpunicastdecoder.h"
#include "decoder/udpmulticastdecoder.h"


//unicast receiver
//gst-launch-1.0 udpsrc port=5200 ! "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96" ! rtph264depay ! h264parse ! decodebin ! videoconvert ! autovideosink sync=false

//multicast receiver
//gst-launch-1.0 udpsrc multicast-group=224.1.1.1 port=5200 ! "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96" ! rtph264depay ! h264parse ! decodebin ! videoconvert ! autovideosink sync=false

QImage drawText(QImage img){
    QPainter p(&img);
    p.setPen(QPen(Qt::red));
    p.setFont(QFont("Times", 100, QFont::Bold));
    p.drawText(img.rect(), Qt::AlignCenter, QDateTime::currentDateTime().time().toString("hh:mm:ss:zzz"));
    return img;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QLabel *lbl =new QLabel;
    QPushButton *btn = new QPushButton("start/stop", lbl);
    lbl->setFixedSize(640,480);
    lbl->show();

    QLabel *reciverLbl = new QLabel("Reciver");
    reciverLbl->setFixedSize(640,480);
    reciverLbl->show();

    UDPMulticastEncoder *stream = new UDPMulticastEncoder();
    stream->config("224.1.1.1", 5200, 640, 480, 20);


    //    UDPUnicastEncoder *stream = new UDPUnicastEncoder();
    //    stream->config("127.0.0.1", 5200, 640, 480, 20);

    QTimer timer;
    QImage img("/home/oscar/Downloads/persons.jpg");
    img = img.scaled(640, 480);
    QObject::connect(&timer, &QTimer::timeout, [stream, lbl, &img](){
        stream->writeImage(drawText(img));
        lbl->setPixmap(QPixmap::fromImage(drawText(img)));
    });

    timer.start(50);

    UDPMulticastDecoder *decoder = new UDPMulticastDecoder();
    decoder->config("224.1.1.1", 5200);

    //    UDPUnicastDecoder *decoder = new UDPUnicastDecoder();
    //    decoder->config("127.0.0.1", 5200);

    QObject::connect(decoder, &UDPUnicastDecoder::onFrame, [reciverLbl](QImage img){
        reciverLbl->setPixmap(QPixmap::fromImage(img));
    });

    QObject::connect(btn, &QPushButton::clicked, [btn, stream, decoder](){
        if(stream->isRunning()){
            stream->stop();
            decoder->stop();
        } else {
            stream->start();
            decoder->start();
        }
    });






    return a.exec();
}
