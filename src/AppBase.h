#pragma once

#include <QObject>
#include <QDebug>
#include <QElapsedTimer>
#include <QCoreApplication>

#include "TaskQueue.h"
#include "TaskGenerator.h"
#include "ConsistencyWarden.h"
#include "NetworkLayer.h"



class AppBase : public QObject
{
    Q_OBJECT;
    friend class ConsistencyWarden;

public:
    AppBase();
    virtual ~AppBase();

    int  main();

signals:
    void  someNewTaskArrived();

protected:
    TaskQueue  _tasks;

private:
    QList<TaskGenerator*>  _generators;
    ConsistencyWarden      _warden;
    QTimer                 _everyLoop;
    QTimer                 _testTime;

protected:
    Publisher  _publisher;

    virtual void  _preMain() = 0;

};


inline
AppBase::AppBase()
    :_warden(this)
    ,_publisher(this)
    ,_everyLoop(this)
    ,_testTime(this)
{
    _testTime.setInterval(10000);
    _testTime.setSingleShot(true);

    for(int i = 0; i < 10; ++i)
    {
        const auto newGenerator = new TaskGenerator();

        connect(newGenerator, &TaskGenerator::newTask, this,
            [this](SlowWorker* task)
            {
                _tasks.put(task);

                emit someNewTaskArrived();
            }
        );

        _generators.append(newGenerator);
    };

    connect(&_everyLoop, &QTimer::timeout, &_warden, &ConsistencyWarden::loopEnds);

    _everyLoop.start();

    connect(&_testTime, &QTimer::timeout, this,
        [this]()
        {
            for(int i = 0; i < _generators.size(); ++i)
                delete _generators[i];

            qApp->quit();
        }
    );

    _testTime.start();
};

inline
AppBase::~AppBase()
{
    if(_tasks.size())
        qInfo() << "-- ERROR: Second data consistency condition not fulfilled";

    auto task = _tasks.get();

    while(task)
    {
        delete task;
        task = _tasks.get();
    };
};

inline
int AppBase::main()
{
    qInfo() << "-- App::preMain()";

    for(int i = 0; i < _generators.size(); ++i)
        _generators.at(i)->start();

    _preMain();

    qInfo() << "-- Begin main event loop";

    const auto result = qApp->exec();

    qInfo() << "-- Finishing main event loop...";

    return result;
};


inline
ConsistencyWarden::ConsistencyWarden(AppBase* app)
    :_loopTime(nullptr)
    ,_started(false)
    ,_taskQueue(&(app->_tasks))
{
    moveToThread(this);

    start();
    _waitStarted();
};
