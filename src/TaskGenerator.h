#pragma once

#include <QThread>
#include <QTimer>
#include <QDebug>

#include "SlowWorker.h"


class TaskGenerator : public QObject
{
    Q_OBJECT;
public:
    TaskGenerator();
    ~TaskGenerator();

    void  start();

signals:
    void  newTask(SlowWorker* task);

private:
    QThread  _thread;
};


inline
TaskGenerator::TaskGenerator()
{
    auto timeout = new QTimer(&_thread);

    timeout->setInterval(/*qrand()%2+*/10);

    connect(timeout, &QTimer::timeout, this,
        [this]()
        {
            if(qrand()%2)
                emit newTask(new SlowWorkerInt(qrand()%3+2));
            else
                emit newTask(new SlowWorkerString(qrand()%3+2));
        }
        , Qt::DirectConnection
    );

    QMetaObject::invokeMethod(timeout, "start", Qt::QueuedConnection);

    qInfo() << "-- TaskGenerator created";
};

inline
TaskGenerator::~TaskGenerator()
{
    _thread.quit();
    _thread.wait();
};

inline
void TaskGenerator::start()
{
    _thread.start();

    qInfo() << "-- TaskGenerator started";
}
