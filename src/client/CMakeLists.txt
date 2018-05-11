project(client)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(CMAKE_AUTOMOC ON)

find_package(Boost 1.56.0 COMPONENTS regex date_time system thread)
find_package(Qt5Widgets)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIR})

file(GLOB client_sources *.cpp)
add_executable(client ${client_sources})

target_link_libraries(client PUBLIC ${Boost_LIBRARIES})
target_link_libraries(client PUBLIC core)
target_link_libraries(client PUBLIC Qt5::Widgets)