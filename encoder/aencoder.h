#ifndef AENCODER_H
#define AENCODER_H

#include <athread.h>
#include "gst/app/gstappsrc.h"
#include "Enums.h"

#include <QElapsedTimer>
#include <QImage>

class AEncoder : public AThread
{
    Q_OBJECT
public:
    explicit AEncoder(QObject *parent = nullptr);

protected:
    inline static const QString GST_APP_SOURCE_STRING = QString("myappsrc");
    virtual void config(QString pIp, short pPort, short pWidth, short pHeight, short pFps) = 0;
    virtual QString createGSTPipeline() = 0;

signals:
    void onError(Enums::GSTError error);

public slots:
    bool writeImage(QImage pImage);

private:
    bool writeRawData(char *data, int len);
    void mainLoop() override;
    GstAppSrc *mAppsrc;
    QElapsedTimer mTimer;
};

#endif // AENCODER_H
