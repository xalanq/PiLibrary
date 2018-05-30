project(core)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(Boost_USE_RELEASE_LIBS ON)
    set(Boost_USE_DEBUG_LIBS OFF)
    set(Boost_USE_DEBUG_RUNTIME OFF)
else()
    set(Boost_USE_RELEASE_LIBS OFF)
    set(Boost_USE_DEBUG_LIBS ON)
    set(Boost_USE_DEBUG_RUNTIME ON)
endif()

find_package(Boost 1.56.0 REQUIRED COMPONENTS system filesystem)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB core_sources *.cpp)
add_library(core ${core_sources})

target_link_libraries(core PUBLIC ${Boost_LIBRARIES})
