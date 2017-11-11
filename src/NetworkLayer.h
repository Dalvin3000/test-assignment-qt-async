#pragma once

#include <QBuffer>
#include <QByteArray>
#include <QMutex>

#include "SlowWorker.h"

class Publisher;


class Receiver : public QObject
{
    Q_OBJECT;
    friend class Publisher;

public:
    static constexpr const char* SOCKET_NAME = "test-server";
    static constexpr int PACKET_SIZE = SlowWorkerString::STRING_SIZE + 2;

    Receiver(QObject* parent);

    QByteArray  readPacket();

private:
    QBuffer     _socket;
    QByteArray  _data;

    static QByteArray&  _sharedData();
    static QMutex&  _sharedDataGuard();

};


class Publisher : public QObject
{
    Q_OBJECT;

public:
    static constexpr const char* SOCKET_NAME = Receiver::SOCKET_NAME;

    Publisher(QObject* parent);

public slots:
    void write(const QString& data);

private:
    QBuffer  _socket;

};


inline
Receiver::Receiver(QObject* parent)
    :QObject(parent)
{ };

inline
QByteArray Receiver::readPacket()
{
    QByteArray result;

    _sharedDataGuard().lock();

    auto& data = _sharedData();

    if(data.size() >= PACKET_SIZE)
    {
        result = data.left(PACKET_SIZE);

        data.remove(0, PACKET_SIZE);
    };

    _sharedDataGuard().unlock();

    return result;
};

inline
QByteArray&  Receiver::_sharedData()
{
    static QByteArray v;

    return v;
};

inline
QMutex&  Receiver::_sharedDataGuard()
{
    static QMutex v;

    return v;
};



inline
Publisher::Publisher(QObject* parent)
    :QObject(parent)
{ };

inline
void Publisher::write(const QString& data)
{
    const auto byteData = "{" + data.toLatin1() + "}";

    for(int i = 0, ie = byteData.size(); i < ie; ++i)
    {
        Receiver::_sharedDataGuard().lock();
        Receiver::_sharedData() += *(byteData.constData()+i);
        Receiver::_sharedDataGuard().unlock();
    };
};
