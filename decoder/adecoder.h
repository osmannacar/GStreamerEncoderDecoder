#ifndef ADECODER_H
#define ADECODER_H

#include "athread.h"
#include <QImage>
#include <gst/gst.h>
#include "Enums.h"

class ADecoder : public AThread
{
    Q_OBJECT
public:
    explicit ADecoder(QObject *parent = nullptr);



    int getWidth() const;
    int getHeight() const;
    int getFPS() const;

signals:
    void onFrame(QImage image);
    void onError(Enums::GSTError error);

protected:
    inline static const QString GST_APP_SINK_STRING = QString("myappsink");
    virtual QString createGSTPipeline() = 0;
    virtual void config(QString pIp, short pPort) = 0;

private:
    void mainLoop() override;
    void extractImage(GstSample *pSample);
    void getVideoInformation(GstSample *pSample);

    int mWidth;
    int mHeight;
    int mFPS;

    const static quint64 PULL_SAMPLE_INTERVAL_NS = 100000000;
    const static int INVALID = -1;
};

#endif // ADECODER_H
