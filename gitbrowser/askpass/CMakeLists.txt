cmake_minimum_required(VERSION 2.6.4)
set(PROJECT askpass)

project(${PROJECT})

set(HEADERS
)
if (UNIX)
    set(SOURCES
        main_unix.cpp
    )
elseif(WIN32)
    set(SOURCES
        main_win.cpp
    )
endif()

set(UI
)

set(RESOURCES
)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
#cmake_policy(SET CMP0028 OLD)
set(CMAKE_CXX_FLAGS "-fPIC")

add_executable(${PROJECT} ${HEADERS} ${SOURCES} ${RESSRC})

if (WIN32)
target_link_libraries(${PROJECT} ws2_32)
endif()

install(TARGETS ${PROJECT} DESTINATION ${CMAKE_BINARY_DIR}/bin)
