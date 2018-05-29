project(core)

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

find_package(Boost 1.56.0 REQUIRED COMPONENTS system filesystem)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB core_sources *.cpp)
add_library(core ${core_sources})

target_link_libraries(core PUBLIC ${Boost_LIBRARIES})
