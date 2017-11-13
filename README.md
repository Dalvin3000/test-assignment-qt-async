# Test assignment 'Async task handling in Qt'

This is a 10-60 min task (depending on your knowledge) that should show your ability to work with an unfamiliar code and/or libraries.

## Application purpose

It's an asynchronous task processor with (semi) low latency. An emulator of task processor, of course. There are several task generators and a consumer that awaits a stream of task processing results packed in a single data stream. The consumer awaits packets of length 32 with all bytes equal. The generators can produce different data types that must be brought to a single form.

## Requirements

The assignment is as simple as possible and doesn't take much time, but has some mandatory restrictions and requirements:
* You can change only files _App.h_ and build script _CMakeLists.txt_ (if needed).
* New files may be added.
* The application must meet these performance requirements:
  1. The task queue size must not overflow.
  2. The main event loop must be responsive to any signals it receives, i.e. loop time must be less than 10ms.
* The application must meet these data consistency of the requirements:
  1. The packets received by network emulation layer must go in one-piece.
  2. All generated packets must be processed.
* The application must finish itself correctly without any exceptions or Qt warnings/errors.

## Current application state

It runs for 10 seconds and then quits. No tasks are processed, no requirements are met. There are no comments in code as if it was written carelessly.

There is a watcher that logs out messages if some of the enumerated requirements above are not met.

_Take into account that the logger or even the whole application may not work properly on a single-core machine/vm._

## Task

Finish application to make it work properly and meet all requirements.

## Build & Run

Dependencies for all platforms:
* [CMake build system](https://cmake.org/download/)
* [Qt 5 framework](http://qt.io/download/)

To build this project you can use a tool by your choice:
* Windows SDK 7.1+
* GNU GCC 6.3+.

### Linux

```bash
cmake .
cmake --build . --config Release
```

### windows

```cmd
SET CMAKE_PREFIX_PATH=/path/to/qt
cmake .
cmake --build . --config Release
```

Where ```/path/to/qt``` points to Qt installation cmake library path. Something like:
```
C:/Qt/5.8.0/5.8/msvc2015_64/lib/cmake
```

If you use Visual Studio then after command ```cmake .``` IDE solution will be generated - it can be used instead of ```cmake --build```.

In case of Qt Creator you can just open CMakeLists.txt with IDE.
