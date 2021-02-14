#ifndef AENCODER_H
#define AENCODER_H

#include <athread.h>
#include "gst/app/gstappsrc.h"

#include <QElapsedTimer>
#include <QImage>

class AEncoder : public AThread
{
public:
    explicit AEncoder(QObject *parent = nullptr);

protected:
    inline static const QString GST_APP_SOURCE_STRING = QString("myappsrc");
    virtual void config(QString pIp, short pPort, short pWidth, short pHeight, short pFps) = 0;
    virtual QString createGSTPipeline() = 0;

public slots:
    bool writeImage(QImage pImage);

private:
    bool writeRawData(char *data, int len);
    void mainLoop() override;
    GstAppSrc *mAppsrc;
    QElapsedTimer mTimer;
};

#endif // AENCODER_H
