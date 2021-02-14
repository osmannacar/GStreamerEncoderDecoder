#ifndef UDPMULTICASTENCODER_H
#define UDPMULTICASTENCODER_H

#include "aencoder.h"

class UDPMulticastEncoder : public AEncoder
{
public:
    explicit UDPMulticastEncoder(QObject *parent = nullptr);
    void config(QString pIp, short pPort, short pWidth, short pHeight, short pFps) override;
    QString createGSTPipeline() override;
private:
    QString mIp;
    short mPort;
    short mWidth;
    short mHeight;
    short mFps;
};

#endif // UDPMULTICASTENCODER_H
