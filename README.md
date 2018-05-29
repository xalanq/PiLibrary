# PiLibrary（皮一下真开心）

A very weak(pi) library system. (一个非常菜(皮)的图书管理系统)

[![GitHub license](https://img.shields.io/github/license/xalanq/PiLibrary.svg)](https://github.com/xalanq/PiLibrary/blob/master/LICENSE)
[![GitHub issues](https://img.shields.io/github/issues/xalanq/PiLibrary.svg)](https://github.com/xalanq/PiLibrary/issues)

## Introduction

It's a cross-platform/multi-thread/async(Server)/sync(Client) library system.

Here are the functions:

* Login/Logout/Register
* Browse Books(Newest books, Top 15 books)
* Search Books(Plain text, Regex)
* Star Books(Star your favorite books)
* Borrow/Return Books
* Record(Browse, Borrow, Keep, Login)
* Administer(Add/Modify book, Modify user)
* Internationalization(supported en_US, zh_CN until now)
* etc

## How to build

**Notice**: We just provide the way to build it with **static** libraries(mongocxx driver), ensure you have already built these static libraries. If you want to build a **dynamic** version, modify `CMakeLists.txt`, `src/core/CMakeLists.txt`, `src/server/CMakeLists.txt`, `src/client/CMakeLists.txt`, `src/test/CMakeLists.txt` yourself.

**Notice**: You should install some dependent libraries firstly. You can check these following links to install them:

* [Qt5 Downloads(needed by Client)](https://www1.qt.io/offline-installers/)
* [Boost Downloads(needed by Client, Server)](https://www.boost.org/users/download/)
* [MongoDB Downloads(needed by Server)](https://www.mongodb.com/download-center)
* [mongocxx driver(needed by Server)](https://mongodb.github.io/mongo-cxx-driver/mongocxx-v3/installation/)
* [My tutorial for Windows 10](http://blog.xalanq.com/win10_vs2017_qt_boost_mongodb_setting/)
* [My tutorial for Linux](http://blog.xalanq.com/linux_qt_boost_mongodb_setting/)

### CMake options

We use `CMake` to build the project. 

Here are some CMake's options:

| Option | Description |
| - | - |
| `-DBUILD_CLIENT=ON` | Build `Client` |
| `-DBUILD_SERVER=ON` | Build `Server` |
| `-DBUILD_BOTH=ON` | Build both `Client` and `Server` |
| `-DBUILD_ALL=ON` | Build both `Client` and `Server` and `Test` |
| `-DCMAKE_BUILD_TYPE=Release` | Build a release version |
| `-DCMAKE_BUILD_TYPE=Debug` | Build a debug version |

### Linux

Use the following commands to build `Server`:

```
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SERVER=ON ..
make
```

### Windows (with Visual Studio)

Use the following commands to build `Client`:

```
cd build
cmake -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT=E:\Boost\boost_1_67_0 -DCMAKE_PREFIX_PATH=E:\mongo\mongo-cxx-driver;E:\mongo\mongo-c-driver;E:\Qt\Qt5.10.1 -DCMAKE_BUILD_TYPE=Release -DBUILD_CLIENT=ON ..
```

**Ensure you have modified the path like `-DBOOST_ROOT=[path]` into your own path.**

Then open `PiLibrary.sln` with VS, and build the solution. Or just use the following command:

```
msbuild.exe /p:Configuration=Release ALL_BUILD.vcxproj
```

## How to run

```
cd ../bin
```

Now, don't tell me you don't know how to run.
