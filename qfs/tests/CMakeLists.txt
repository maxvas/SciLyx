cmake_minimum_required(VERSION 2.6.4)
set(PROJECT qfs-tests)

project(${PROJECT})

find_package( Qt5 REQUIRED COMPONENTS Core Gui Widgets Network)

set(HEADERS
)

set(SOURCES
    main.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
qt5_wrap_cpp (MOC_SOURCES ${HEADERS})
qt5_wrap_ui (UIS ${UI})
qt5_add_resources (RESSRC ${RESOURCES})
cmake_policy(SET CMP0028 OLD)
set(CMAKE_CXX_FLAGS "-fPIC")

#add_library ( ${PROJECT} SHARED ${HEADERS} ${SOURCES} ${MOC_SOURCES})

add_executable(${PROJECT} ${HEADERS} ${SOURCES} ${MOC_SOURCES})

target_link_libraries(${PROJECT} Qt5:Core qfs)
