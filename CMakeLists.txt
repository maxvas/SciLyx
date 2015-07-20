cmake_minimum_required(VERSION 2.6.4)
set(PROJECT SciLyx)

project(${PROJECT})
add_subdirectory(gitbrowser)
add_subdirectory(lyxwidget)
add_subdirectory(sampleapp)
add_subdirectory(qfs)
add_dependencies(SciLyxGui lyxwidget gitbrowser)

find_package( Qt5 REQUIRED COMPONENTS Core Gui Widgets)

SET(CMAKE_SHARED_LIBRARY_PREFIX "")

SET (SOURCES
    scilyx.cpp
    scilyxplugin.cpp
)

SET (HEADERS
    scilyx.h
    scilyxplugin.h
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
SET (MOC_SOURCES )
qt5_wrap_cpp (MOC_SOURCES ${HEADERS})

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

add_library ( ${PROJECT} SHARED ${HEADERS} ${SOURCES} ${MOC_SOURCES})

target_link_libraries(${PROJECT} Qt5::Widgets lyxwidget gitbrowser qfs)
install(TARGETS ${PROJECT} DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(WRITE ${CMAKE_BINARY_DIR}/pathes.conf "scilyx=${CMAKE_BINARY_DIR}/bin" )
install (FILES ${CMAKE_BINARY_DIR}/pathes.conf DESTINATION ${CMAKE_BINARY_DIR}/bin)
