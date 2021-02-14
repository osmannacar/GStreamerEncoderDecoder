#ifndef BDIGITALVIDEODECODER_H
#define BDIGITALVIDEODECODER_H

#include "adecoder.h"

class UDPMulticastDecoder : public ADecoder
{
public:
    explicit UDPMulticastDecoder(QObject *parent = nullptr);
    void config(QString pIp, short pPort) override;

private:
    QString createGSTPipeline() override;
    QString mIp;
    short mPort;

};

#endif // BDIGITALVIDEODECODER_H
