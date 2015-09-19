cmake_minimum_required(VERSION 2.6.4)
set(PROJECT matplugin)

find_package( Qt5 REQUIRED COMPONENTS Core Gui Widgets)

set(HEADERS
    matwindow.h
    matplugin.h
)

set(SOURCES
    matwindow.cpp
    matplugin.cpp
)

set(UI
    matwindow.ui
)

set(RESOURCES
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../..)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET (MOC_SOURCES )
SET (UIS )
SET (RESSRC )
qt5_wrap_cpp (MOC_SOURCES ${HEADERS})
qt5_wrap_ui (UIS ${UI})
qt5_add_resources (RESSRC ${RESOURCES})

if(COMMAND cmake_policy)
    if(POLICY CMP0028)
        cmake_policy(SET CMP0028 OLD)
    endif()
    if(POLICY CMP0020)
        cmake_policy(SET CMP0020 OLD)
    endif()
endif()

if (${UNIX})
    set(CMAKE_CXX_FLAGS "-fPIC")
endif (${UNIX})

add_library(${PROJECT} SHARED ${HEADERS} ${SOURCES} ${MOC_SOURCES} ${UIS} ${RESSRC})
qt5_use_modules(${PROJECT} Widgets Core Gui)
target_link_libraries(${PROJECT} SciLyx)

install(TARGETS ${PROJECT} DESTINATION ${CMAKE_BINARY_DIR}/bin/scilyx_plugins)
