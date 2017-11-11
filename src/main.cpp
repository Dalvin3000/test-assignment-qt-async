#include "App.h"

#include <QCoreApplication>
#include <ctime>

#if defined(Q_OS_WIN32)
#include <Windows.h>
#endif
#include <iostream>


int main(int argc, char** argv)
{
    int result;

#if defined(Q_OS_WIN32)
    timeBeginPeriod(1);
#endif

    qsrand((uint)std::clock());

    {
        QCoreApplication qt(argc, argv);

        App app;

        result = app.main();
    };

    qInfo() << "-- Exiting process";

#if defined(Q_OS_WIN32)
    timeEndPeriod(1);
#endif

    return result;
}
