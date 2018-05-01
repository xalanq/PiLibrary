# PiLibrary（皮一下真开心）

A very weak(pi) library system. (一个非常菜(皮)的图书管理系统)

## How to build

**Notice**: we just provide the way to build it with **static** libraries(mongocxx driver), ensure you have already built these static libraries. If you want to build a **dynamic** version, modify `src/server/CMakeLists.txt` yourself.

### Linux

Use commands like that:

```
cd build
cmake -DBOOST_ROOT=/usr/lib/boost -DCMAKE_PREFIX_PATH=/usr/lib/mongo-cxx-driver -DCMAKE_BUILD_TYPE=Release ..
```

Ensure you have modified the path like `-DBOOST_ROOT=[path]` to your own path.

Then:

```
make
```

### Windows(with Visual Studio)

Use commands like that:

```
cd build
cmake -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT=E:\Boost\boost_1_67_0 -DCMAKE_PREFIX_PATH=E:\mongo\mongo-cxx-driver -DCMAKE_BUILD_TYPE=Release ..
```

Ensure you have modified the path like `-DBOOST_ROOT=[path]` to your own path.

Then open `PiLibrary.sln` with VS, and build solution, or use following commands:

```
msbuild.exe /p:Configuration=Release ALL_BUILD.vcxproj
```

## How to run

Don't tell me you don't know how to run.


