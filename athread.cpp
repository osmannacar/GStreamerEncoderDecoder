#include "athread.h"
#include <QDebug>

AThread::AThread(QObject *parent) : QObject(parent)
{
}

AThread::~AThread() {
    stop();
    moveToThread(QThread::currentThread());
}

void AThread::start() {
    if(mIsRunning){
        return;
    }
    moveToThread(&mThread);
    connect(&mThread, SIGNAL(started()), this, SLOT(outerLoop()));
    mIsRunning = true;
    mThread.start();
}

void AThread::stop() {
    if(!mIsRunning){
        return;
    }
    mIsRunning = false;
    mThread.wait();
}

bool AThread::isRunning() const
{
    return mIsRunning;
}

void AThread::outerLoop() {
    mainLoop();
    mThread.quit();
    mIsRunning = false;
    disconnect(&mThread, SIGNAL(started()), this, SLOT(outerLoop()));
}
