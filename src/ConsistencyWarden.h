#pragma once

#include <QThread>
#include <QEventLoop>

#include "TaskQueue.h"
#include "NetworkLayer.h"

class AppBase;


class ConsistencyWarden : public QThread
{
    Q_OBJECT;

public:
    ConsistencyWarden(AppBase* app);
    ~ConsistencyWarden();

public slots:
    void loopEnds();

private:
    QElapsedTimer*  _loopTime;
    Receiver*       _receiver;
    bool            _started;

    TaskQueue*      _taskQueue;

    void  run() override;

    void  _waitStarted();

};


inline
ConsistencyWarden::~ConsistencyWarden()
{
    requestInterruption();
    wait();
};

inline
void ConsistencyWarden::loopEnds()
{
    if(_loopTime)
        _loopTime->restart();
};

inline
void ConsistencyWarden::run()
{
    QEventLoop loop;

    _receiver = new Receiver(this);

    _loopTime = new QElapsedTimer();
    _loopTime->start();

    _started = true;

    while(!isInterruptionRequested())
    {
        if(_loopTime)
        {
            const auto loopTime = _loopTime->elapsed();
            const auto queueSize = _taskQueue->size();

            if(queueSize > 1000)
            {
                qInfo() << "-- ERROR: First performance condition not fulfilled. Queue size:" << queueSize;
                QThread::msleep(1000);
            };

            if(loopTime > 10)
            {
                qInfo() << "-- ERROR: Second performance condition not fulfilled";
                _loopTime->restart();
            };
        };

        const auto packet = _receiver->readPacket();

        if(packet.size())
        {
            bool isValid = true;
            int  packetSize = packet.size();

            if(packetSize != Receiver::PACKET_SIZE)
                isValid = false;
            else if(packet.at(0) != '{' || packet.at(Receiver::PACKET_SIZE-1) != '}')
                isValid = false;
            else
            {
                const auto ch = packet.at(1);

                for(int i = 2, ie = Receiver::PACKET_SIZE-2; i < ie; ++i)
                    if(packet.at(i) != ch)
                    {
                        isValid = false;
                        break;
                    };
            };

            if(!isValid)
            {
                qInfo() << "-- ERROR: First data consistency condition not fulfilled. Packet:" << packet;
                QThread::msleep(1000);
            }
            //else
            //    qInfo() << "-- Task result received:" << packet;
        };

        loop.processEvents(QEventLoop::ExcludeUserInputEvents);
        QThread::yieldCurrentThread();
    };

    delete _receiver;
    delete _loopTime;
};

inline
void ConsistencyWarden::_waitStarted()
{
    while(!_started);
};
