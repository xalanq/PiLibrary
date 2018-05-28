project(server)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.56.0 COMPONENTS regex date_time system thread)
find_package(libmongocxx-static 3.2.0 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIR})

file(GLOB_RECURSE server_sources *.cpp)
add_executable(server ${server_sources})

target_link_libraries(server PUBLIC ${Boost_LIBRARIES})
target_link_libraries(server PUBLIC core)
target_include_directories(server PUBLIC ${LIBMONGOCXX_STATIC_INCLUDE_DIRS})
target_link_libraries(server PUBLIC ${LIBMONGOCXX_STATIC_LIBRARIES})
target_compile_definitions(server PUBLIC ${LIBMONGOCXX_STATIC_DEFINITIONS})

list(FIND LIBMONGOCXX_STATIC_DEFINITIONS "BSONCXX_STATIC" LIST_IDX)
if (${LIST_IDX} EQUAL -1)
    message(FATAL_ERROR "Expected BSONCXX_STATIC to be defined")
endif()
list(FIND LIBMONGOCXX_STATIC_DEFINITIONS "MONGOCXX_STATIC" LIST_IDX)
if (${LIST_IDX} EQUAL -1)
    message(FATAL_ERROR "Expected MONGOCXX_STATIC to be defined")
endif()