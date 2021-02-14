#ifndef UDPUNICASTDECODER_H
#define UDPUNICASTDECODER_H

#include "adecoder.h"

class UDPUnicastDecoder : public ADecoder
{
public:
    explicit UDPUnicastDecoder(QObject *parent = nullptr);
    void config(QString pIp, short pPort) override;
private:
    QString createGSTPipeline() override;
    QString mIp;
    short mPort;
};

#endif // UDPUNICASTDECODER_H
