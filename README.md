# PiLibrary（皮一下真开心）

[![Build Status](https://travis-ci.org/xalanq/PiLibrary.svg?branch=master)](https://travis-ci.org/xalanq/PiLibrary)
[![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](https://github.com/xalanq/PiLibrary/blob/master/LICENSE)
[![GitHub version](https://badge.fury.io/gh/xalanq%2FPiLibrary.svg)](https://github.com/xalanq/PiLibrary/)

A very weak(pi) library system. (一个非常菜(皮)的图书管理系统)

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
* etc.

## Interface


Log in             |  Sign up
:-------------------------:|:-------------------------:
![](https://raw.githubusercontent.com/xalanq/PiLibrary/master/doc/images/login.png)  |  ![](https://raw.githubusercontent.com/xalanq/PiLibrary/master/doc/images/sign-up.png)


Newest books             |  Search
:-------------------------:|:-------------------------:
![](https://raw.githubusercontent.com/xalanq/PiLibrary/master/doc/images/newest-book.png)  |  ![](https://raw.githubusercontent.com/xalanq/PiLibrary/master/doc/images/search.png)


Book             |  Borrow a book
:-------------------------:|:-------------------------:
![](https://raw.githubusercontent.com/xalanq/PiLibrary/master/doc/images/book.png)  |  ![](https://raw.githubusercontent.com/xalanq/PiLibrary/master/doc/images/borrow-book.png)


Add a book             |  Administer
:-------------------------:|:-------------------------:
![](https://raw.githubusercontent.com/xalanq/PiLibrary/master/doc/images/add-book.png)  |  ![](https://raw.githubusercontent.com/xalanq/PiLibrary/master/doc/images/administer.png)


Setting             |  About
:-------------------------:|:-------------------------:
![](https://raw.githubusercontent.com/xalanq/PiLibrary/master/doc/images/setting.png)  |  ![](https://raw.githubusercontent.com/xalanq/PiLibrary/master/doc/images/about.png)


## How to build

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
| `-DBUILD_CLIENT_X=ON` | Build `Client` |
| `-DBUILD_SERVER_X=ON` | Build `Server` |
| `-DBUILD_BOTH_X=ON` | Build both `Client` and `Server`(default) |
| `-DBUILD_ALL_X=ON` | Build both `Client` and `Server` and `Test` |
| `-DBUILD_STATIC_X=ON` | Build a static version(default is dynamic version) |
| `-DCMAKE_BUILD_TYPE=Release` | Build a release version(default) |
| `-DCMAKE_BUILD_TYPE=Debug` | Build a debug version |

### Linux & OSX

Example:

Use the following commands to build `Server`(static version):

```
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SERVER_X=ON -DBUILD_STATIC_X=ON ..
make
```

### Windows (with Visual Studio)

Example:

Use the following commands to build `Client`(dynamic version):

```
cd build
cmake -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT="E:\Boost\boost_1_67_0" -DCMAKE_PREFIX_PATH="E:\mongo\mongo-cxx-driver;E:\mongo\mongo-c-driver;E:\Qt\Qt5.10.1" -DCMAKE_BUILD_TYPE=Release -DBUILD_CLIENT_X=ON ..
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

**Note**: Server connects port 23332 to MongoDB as default. If you want to change that port, edit `config.json` after server's first run.

**Note**: If you want to run server on silent mode, just redirect `stderr` to a file. For example `./server 2> log`.

## More information

Go to [Wiki](https://github.com/xalanq/PiLibrary/wiki)
