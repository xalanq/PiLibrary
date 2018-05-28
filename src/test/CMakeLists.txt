project(testServer)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/test)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.56.0 COMPONENTS regex date_time system thread)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIR})

add_executable(testServer1 testServer1.cpp ../server/utils.cpp)
add_executable(testServer2 testServer2.cpp ../server/utils.cpp)
add_executable(testServer3 testServer3.cpp ../server/utils.cpp)
add_executable(testServer4 testServer4.cpp ../server/utils.cpp)

target_link_libraries(testServer1 PUBLIC ${Boost_LIBRARIES})
target_link_libraries(testServer1 PUBLIC core)
target_link_libraries(testServer2 PUBLIC ${Boost_LIBRARIES})
target_link_libraries(testServer2 PUBLIC core)
target_link_libraries(testServer3 PUBLIC ${Boost_LIBRARIES})
target_link_libraries(testServer3 PUBLIC core)
target_link_libraries(testServer4 PUBLIC ${Boost_LIBRARIES})
target_link_libraries(testServer4 PUBLIC core)
