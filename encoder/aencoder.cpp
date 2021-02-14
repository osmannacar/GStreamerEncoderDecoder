#include "aencoder.h"

#include "gst/gst.h"
#include "gst/app/gstappsrc.h"
#include "gst/gstelement.h"

#include <QDebug>
#include <QApplication>
#include <QAbstractEventDispatcher>

AEncoder::AEncoder(QObject *parent)
    : AThread(parent)
    , mAppsrc(nullptr)
{

}

bool AEncoder::writeImage(QImage pImage)
{
    if(!isRunning()){
        qDebug() << "AEncoder::writeImage record thread not started";
        return false;
    }
    if(!mAppsrc) {
        qDebug() << "AEncoder::writeImage appsrc is empty!!!!";
        emit onError(Enums::GSTError::APPSRC_NULL);
        return false;
    }
    return writeRawData((char*)pImage.constBits(), pImage.sizeInBytes());
}

bool AEncoder::writeRawData(char *data, int len)
{
    GstBuffer *buffer = gst_buffer_new_and_alloc(len);
    gst_buffer_fill(buffer, 0, data, len);

    GST_BUFFER_PTS(buffer) = mTimer.nsecsElapsed();
    GST_BUFFER_DTS(buffer) = mTimer.nsecsElapsed();

    int result = gst_app_src_push_buffer(GST_APP_SRC(mAppsrc), buffer);

    if(result != GST_FLOW_OK) {
        qDebug() << "AEncoder::writeRawData push buffer returns: " << result << "!";
        emit onError(Enums::GSTError::PUSH_BUFFER);
        return false;
    }
    return true;
}

void AEncoder::mainLoop()
{
    if( !gst_is_initialized() ) {
        gst_init(nullptr, nullptr);
    }

    QString pipelineStr = createGSTPipeline();
    if(pipelineStr.isEmpty()){
        qDebug() << "AEncoder::mainLoop pipelineStr is empty";
        emit onError(Enums::GSTError::PIPELINESTR_EMPTY);
        return;
    }
    qDebug() << "AEncoder::mainLoop pipelineStr: " << pipelineStr;

    GstElement *gstPipeline = gst_parse_launch(pipelineStr.toStdString().c_str(), nullptr);
    mAppsrc = (GstAppSrc*) gst_bin_get_by_name(GST_BIN(gstPipeline), GST_APP_SOURCE_STRING.toStdString().c_str());
    if(!mAppsrc) {
        emit onError(Enums::GSTError::APPSRC_NULL);
        qDebug() << "AEncoder::mainLoop mAppsrc is empty!";
        return;
    }

    mTimer.start();
    gst_element_set_state(gstPipeline, GST_STATE_PLAYING);
    while(isRunning()) {
        if(QThread::currentThread()->eventDispatcher()->hasPendingEvents())
            qApp->processEvents();
        else
            QThread::msleep(1);
    }
    gst_app_src_end_of_stream(mAppsrc);
    gst_element_set_state (gstPipeline, GST_STATE_NULL);

    gst_object_unref(mAppsrc);
    g_object_unref(gstPipeline);
    mAppsrc = nullptr;

}
