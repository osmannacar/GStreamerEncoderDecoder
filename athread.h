#ifndef ATHREAD_H
#define ATHREAD_H

#include <QObject>
#include <QThread>

class AThread : public QObject
{
    Q_OBJECT
public:
    explicit AThread(QObject *parent = nullptr);
    virtual ~AThread();

    void start();
    void stop();

    bool isRunning() const;
protected:
    virtual void mainLoop() = 0;

private slots:
    void outerLoop();

private:
    bool mIsRunning = false;
    QThread mThread;
};

#endif // ATHREAD_H
