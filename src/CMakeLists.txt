add_subdirectory(core)

if (BUILD_CLIENT OR BUILD_BOTH OR BUILD_ALL)
    add_subdirectory(client)
endif()

if (BUILD_SERVER OR BUILD_BOTH OR BUILD_ALL)
    add_subdirectory(server)
endif()

if (BUILD_SERVER_TEST OR BUILD_ALL)
    add_subdirectory(test)
endif()