#ifndef UDPUNICASTENCODER_H
#define UDPUNICASTENCODER_H

#include "aencoder.h"
class UDPUnicastEncoder : public AEncoder
{
public:
    explicit UDPUnicastEncoder(QObject *parent = nullptr);
    void config(QString pIp, short pPort, short pWidth, short pHeight, short pFps) override;
    QString createGSTPipeline() override;
private:
    QString mIp;
    short mPort;
    short mWidth;
    short mHeight;
    short mFps;
};

#endif // UDPUNICASTENCODER_H
