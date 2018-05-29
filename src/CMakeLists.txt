add_subdirectory(core)

option(BUILD_CLIENT "Build client" OFF)
option(BUILD_SERVER "Build server" OFF)
option(BUILD_BOTH "Build server, client" ON)
option(BUILD_ALL "Build server, client, test" OFF)

if (BUILD_CLIENT OR BUILD_BOTH OR BUILD_ALL)
    add_subdirectory(client)
endif()

if (BUILD_SERVER OR BUILD_BOTH OR BUILD_ALL)
    add_subdirectory(server)
endif()

if (BUILD_SERVER_TEST OR BUILD_ALL)
    add_subdirectory(test)
endif()