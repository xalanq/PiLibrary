project(testServer)

add_executable(testServer1 test1.cpp)
add_executable(testServer2 test2.cpp)

SET(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.56.0 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIR})

message(STATUS "include dir ${Boost_INCLUDE_DIR}")
message(STATUS "libraries dir ${Boost_LIBRARIES}")

target_link_libraries(testServer1 PUBLIC ${Boost_LIBRARIES})
add_dependencies(testServer1 server)
target_link_libraries(testServer2 PUBLIC ${Boost_LIBRARIES})
add_dependencies(testServer2 server)