# PiLibrary（皮一下真开心）

A very weak(pi) library system. (一个非常菜(皮)的图书管理系统)

## How to build

**Notice**: We just provide the way to build it with **static** libraries(mongocxx driver), ensure you have already built these static libraries. If you want to build a **dynamic** version, modify `CMakeLists.txt`, `src/core/CMakeLists.txt`, `src/server/CMakeLists.txt`, `src/server/test/CMakeLists.txt` yourself.

**Notice**: You should install `Boost` and `MongoDB` firstly. You can check these following links to install them:

* [Boost Downloads](https://www.boost.org/users/download/)
* [MongoDB Downloads](https://www.mongodb.com/download-center)
* [mongocxx driver](https://mongodb.github.io/mongo-cxx-driver/mongocxx-v3/installation/)
* [My tutorial](http://blog.xalanq.com/win10_vs2017_qt_boost_mongodb_setting/)

### Linux

Use commands like these:

```
cd build
cmake -DBOOST_ROOT=/usr/lib/boost -DCMAKE_PREFIX_PATH=/usr/lib/mongo-cxx-driver;/usr/lib/mongo-c-driver;/usr/lib/qt5 -DCMAKE_BUILD_TYPE=Release ..
```

Ensure you have modified the path like `-DBOOST_ROOT=[path]` into your own path.

Then:

```
make
```

### Windows (with Visual Studio)

Use commands like these:

```
cd build
cmake -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT=E:\Boost\boost_1_67_0 -DCMAKE_PREFIX_PATH=E:\mongo\mongo-cxx-driver;E:\mongo\mongo-c-driver;E:\Qt\Qt5.10.1 -DCMAKE_BUILD_TYPE=Release ..
```

Ensure you have modified the path like `-DBOOST_ROOT=[path]` into your own path.

Then open `PiLibrary.sln` with VS, and build solution. Or just use the following command:

```
msbuild.exe /p:Configuration=Release ALL_BUILD.vcxproj
```

## How to run

```
cd ../bin
```

Now, don't tell me you don't know how to run.


