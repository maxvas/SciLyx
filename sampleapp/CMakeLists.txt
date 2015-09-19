cmake_minimum_required(VERSION 2.6.4)
set(PROJECT SciLyxGui)

project(${PROJECT})

find_package( Qt5 REQUIRED COMPONENTS Core Quick Declarative Gui Sql Widgets Network)

#add_subdirectory(sampleplugin)
add_subdirectory(eqplugin)
add_subdirectory(matplugin)

set(HEADERS
)

set(SOURCES
    main.cpp
)

set(UI
)

set(RESOURCES
)
#LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/..)
#LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../gitbrowser)
#LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../lyxwidget)
#LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../qfs)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/..)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../qfs)
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

add_executable(${PROJECT} ${HEADERS} ${SOURCES} ${MOC_SOURCES} ${UIS} ${RESSRC})
qt5_use_modules(${PROJECT} Widgets Quick Declarative Core Network Sql Gui)
target_link_libraries(${PROJECT} SciLyx)

install(TARGETS ${PROJECT} DESTINATION ${CMAKE_BINARY_DIR}/bin)
