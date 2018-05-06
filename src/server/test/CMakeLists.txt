project(testServer)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/test)

SET(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.56.0 COMPONENTS regex date_time system thread)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIR})

add_executable(testServer1 test1.cpp)
add_executable(testServer2 test2.cpp)
add_executable(testServer3 test3.cpp)

target_link_libraries(testServer1 PUBLIC ${Boost_LIBRARIES})
target_link_libraries(testServer1 PUBLIC core)
target_link_libraries(testServer2 PUBLIC ${Boost_LIBRARIES})
target_link_libraries(testServer2 PUBLIC core)
target_link_libraries(testServer3 PUBLIC ${Boost_LIBRARIES})
target_link_libraries(testServer3 PUBLIC core)