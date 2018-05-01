project(core)

file(GLOB core_sources *.cpp)
add_library(core ${core_sources})

SET(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.56.0 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIR})

target_link_libraries(core PUBLIC ${Boost_LIBRARIES})
