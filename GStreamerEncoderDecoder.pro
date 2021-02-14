QT += gui core

CONFIG += c++17 console
CONFIG -= app_bundle

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH +=  /usr/include/gstreamer-1.0 \
                /usr/include/glib-2.0 \
                /usr/lib/x86_64-linux-gnu/glib-2.0/include /

LIBS += -lgstreamer-1.0 -lglib-2.0 -lgobject-2.0 -lgstapp-1.0


SOURCES += \
        athread.cpp \
        decoder/adecoder.cpp \
        decoder/udpmulticastdecoder.cpp \
        decoder/udpunicastdecoder.cpp \
        encoder/aencoder.cpp \
        encoder/udpmulticastencoder.cpp \
        encoder/udpunicastencoder.cpp \
        main.cpp \


HEADERS += \
    Enums.h \
    athread.h \
    decoder/adecoder.h \
    decoder/udpmulticastdecoder.h \
    decoder/udpunicastdecoder.h \
    encoder/aencoder.h \
    encoder/udpmulticastencoder.h \
    encoder/udpunicastencoder.h



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
