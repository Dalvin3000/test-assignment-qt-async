#pragma once

#include <QThread>
#include <QString>
#include <QVariant>


class SlowWorker
{
public:
    SlowWorker(int seed);
    //virtual ~SlowWorker() {};

    virtual QVariant  doWork() = 0;

    bool  isDone() const  { return _done; }

protected:
    int _seed;
    bool _done;

};


class SlowWorkerInt : public SlowWorker
{
public:
    SlowWorkerInt(int seed);

    QVariant  doWork() override;

private:

};


class SlowWorkerString : public SlowWorker
{
public:
    static constexpr int STRING_SIZE = 32;

    SlowWorkerString(int seed);

    QVariant  doWork() override;

private:

};


inline
SlowWorker::SlowWorker(int seed)
    :_seed(seed)
    ,_done(false)
{ };


inline
SlowWorkerInt::SlowWorkerInt(int seed)
    :SlowWorker(seed)
{ };

inline
QVariant SlowWorkerInt::doWork()
{
    QThread::msleep(/*_seed**/3);

    _done = true;

    return (int)(qrand()%30+(int)'a');
};


inline
SlowWorkerString::SlowWorkerString(int seed)
    :SlowWorker(seed)
{ };

inline
QVariant SlowWorkerString::doWork()
{
    QThread::msleep(/*_seed**/3);

    static const QString chars("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    static const int resultLen = STRING_SIZE;
    static const int charsLen = chars.length();

    QString result;

    const auto ch = chars.at(qrand() % charsLen);

    for(int i=0 ; i < resultLen; ++i)
        result += ch;

    _done = true;

    return result;
};
