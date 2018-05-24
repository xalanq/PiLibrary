project(core)

file(GLOB core_sources *.cpp)
add_library(core ${core_sources})

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.56.0 COMPONENTS system filesystem)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIR})

target_link_libraries(core PUBLIC ${Boost_LIBRARIES})
