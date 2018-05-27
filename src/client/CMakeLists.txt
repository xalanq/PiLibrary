project(client)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Boost 1.56.0 COMPONENTS regex date_time system thread)
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIR})

file(GLOB_RECURSE client_sources *.cpp *h *.qrc)
add_executable(client ${client_sources})

target_link_libraries(client core ${Boost_LIBRARIES} Qt5::Widgets Qt5::Core Qt5::Gui)