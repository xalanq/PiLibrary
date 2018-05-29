project(server)

option(BUILD_STATIC "Build static version" OFF)

set(Boost_USE_MULTITHREADED ON)

if (BUILD_STATIC)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_STATIC_RUNTIME ON)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(Boost_USE_DEBUG_RUNTIME OFF)
else()
    set(Boost_USE_DEBUG_RUNTIME ON)
endif()

find_package(Boost 1.56.0 COMPONENTS regex date_time system thread)

if (BUILD_STATIC)
    find_package(libmongocxx-static 3.2.0 REQUIRED)
else()
    find_package(libmongocxx 3.2.0 REQUIRED)
endif()

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIR})

file(GLOB_RECURSE server_sources *.cpp)
add_executable(server ${server_sources})

target_link_libraries(server PUBLIC ${Boost_LIBRARIES})
target_link_libraries(server PUBLIC core)

if (BUILD_STATIC)
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
else()
    target_include_directories(server PUBLIC ${LIBMONGOCXX_INCLUDE_DIRS})
    target_link_libraries(server PUBLIC ${LIBMONGOCXX_LIBRARIES})
    target_compile_definitions(server PUBLIC ${LIBMONGOCXX_DEFINITIONS})
    
    list(FIND LIBMONGOCXX_DEFINITIONS "BSONCXX_STATIC" LIST_IDX)
    if (${LIST_IDX} GREATER -1)
        message(FATAL_ERROR "Expected BSONCXX_STATIC to not be defined")
    endif()
    list(FIND LIBMONGOCXX_DEFINITIONS "MONGOCXX_STATIC" LIST_IDX)
    if (${LIST_IDX} GREATER -1)
        message(FATAL_ERROR "Expected MONGOCXX_STATIC to not be defined")
    endif()
endif()