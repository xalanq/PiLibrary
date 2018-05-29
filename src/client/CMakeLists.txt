project(client)

if (BUILD_STATIC)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_STATIC_RUNTIME ON)
else()
    add_definitions(-DBOOST_ALL_DYN_LINK)
    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_STATIC_RUNTIME OFF)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(Boost_USE_RELEASE_LIBS ON)
    set(Boost_USE_DEBUG_RUNTIME OFF)
else()
    set(Boost_USE_RELEASE_LIBS OFF)
    set(Boost_USE_DEBUG_RUNTIME ON)
endif()

set(Boost_USE_MULTITHREADED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Boost 1.56.0 REQUIRED COMPONENTS regex date_time system thread)
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE client_sources *.cpp *h *.qrc)
add_executable(client ${client_sources})

target_link_libraries(client core ${Boost_LIBRARIES} Qt5::Widgets Qt5::Core Qt5::Gui)