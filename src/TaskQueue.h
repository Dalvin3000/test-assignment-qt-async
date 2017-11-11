#pragma once

#include <QList>
#include <QAtomicInt>

#include "SlowWorker.h"


class TaskQueue
{
public:
    TaskQueue();

    void  put(SlowWorker* newTask);

    SlowWorker*  get();
    int  size();

private:
    QList<SlowWorker*>  _tasks;
    QAtomicInt          _countCache;

};


inline
TaskQueue::TaskQueue()
    :_countCache(0)
{ };

inline
void TaskQueue::put(SlowWorker* newTask)
{
    _tasks.append(newTask);
    _countCache.ref();
};

inline
SlowWorker* TaskQueue::get()
{
    if(_tasks.size())
    {
        _countCache.deref();
        return _tasks.takeFirst();
    };

    return 0;
};

inline
int TaskQueue::size()
{
    return _countCache;
};
