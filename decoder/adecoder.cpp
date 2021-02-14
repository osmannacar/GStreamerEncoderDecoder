#include "adecoder.h"
#include <gst/app/gstappsink.h>
#include <QDebug>

static void gstBufferCleanupHandler(void *info) {
    GstBuffer *buffer = (GstBuffer*)info;
    gst_buffer_unref(buffer);
}

ADecoder::ADecoder(QObject *parent)
    : AThread(parent)
    , mWidth(INVALID)
    , mHeight(INVALID)
    , mFPS(INVALID)
{

}

int ADecoder::getFPS() const
{
    return mFPS;
}

int ADecoder::getHeight() const
{
    return mHeight;
}

int ADecoder::getWidth() const
{
    return mWidth;
}

void ADecoder::mainLoop()
{
    // clean info
    mFPS = INVALID;
    mHeight = INVALID;
    mWidth = INVALID;

    if( !gst_is_initialized() ) {
        gst_init(nullptr, nullptr);
    }
    QString pipelineStr = createGSTPipeline();
    if(pipelineStr.isEmpty()){
        qDebug() << "ADecoder::mainLoop pipelineStr is empty !!!!";
        emit onError(Enums::GSTError::PIPELINESTR_EMPTY);
        return;
    }
    qDebug() << "ADecoder::mainLoop pipelineStr:" << pipelineStr;

    GstElement* tGstPipeline = gst_parse_launch(pipelineStr.toStdString().c_str(), nullptr);
    if(!tGstPipeline){
        qDebug() << "ADecoder::mainLoop error pipleline null !!!!";
        emit onError(Enums::GSTError::PIPELINE_NULL);
        return;
    }

    GstAppSink *tAppsink = (GstAppSink*) gst_bin_get_by_name(GST_BIN(tGstPipeline), GST_APP_SINK_STRING.toStdString().c_str());
    if(!tAppsink){
        qDebug() << "ADecoder::mainLoop error tAppsink null !!!!";
        g_object_unref(tGstPipeline);
        emit onError(Enums::GSTError::APPSINK_NULL);
        return;
    }

    gst_element_set_state(tGstPipeline, GST_STATE_PLAYING);

    while(isRunning()) {
        GstSample *pSample = gst_app_sink_try_pull_sample(tAppsink, PULL_SAMPLE_INTERVAL_NS);
        if(pSample){
            getVideoInformation(pSample);
            extractImage(pSample);
            gst_sample_unref(pSample);
        } else if(gst_app_sink_is_eos(tAppsink)) {
            qDebug() << "eos arrived";
            emit onError(Enums::GSTError::EOS);
            break;
        }
    }

    gst_element_set_state (tGstPipeline, GST_STATE_NULL);

    g_object_unref(tAppsink);
    g_object_unref(tGstPipeline);
}

void ADecoder::extractImage(GstSample *pSample)
{
    GstBuffer *buffer = gst_sample_get_buffer(pSample);
    if(buffer == nullptr){
        emit onError(Enums::GSTError::BUFFER_NULL);
        return;
    }

    GstMapInfo mapInfo;
    gst_buffer_map(buffer, &mapInfo, GST_MAP_READ);

    gst_buffer_ref(buffer);
    QImage img = QImage(mapInfo.data, mWidth, mHeight, QImage::Format_RGB888, gstBufferCleanupHandler, buffer);

    if(img.isNull()) {
        emit onError(Enums::GSTError::IMAGE_NULL);
    } else {
        emit onFrame(img);
    }

    gst_buffer_unmap(buffer, &mapInfo);
}

void ADecoder::getVideoInformation(GstSample *pSample)
{
    if((mFPS != INVALID) && (mHeight != INVALID) && (mWidth != INVALID)) {
        return;
    }

    int num, denom;
    GstCaps *tCaps = gst_sample_get_caps(pSample);
    GstStructure *tStructure = gst_caps_get_structure(tCaps, 0);
    gst_structure_get_int (tStructure, "width", &mWidth);
    gst_structure_get_int (tStructure, "height", &mHeight);
    gst_structure_get_fraction (tStructure, "framerate", &num, &denom);
    mFPS = float(num)/float(denom);
    qDebug() <<"fps:" <<mFPS  <<"mRealWidth:" <<mWidth  <<"mRealHeight:" <<mHeight;
}
