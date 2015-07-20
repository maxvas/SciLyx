cmake_minimum_required(VERSION 2.6.4)
set(PROJECT qfs)

project(${PROJECT})

find_package( Qt5 REQUIRED COMPONENTS Core Gui Widgets Network)

set(HEADERS
    qfilemanager.h
    qfileremover.h
    qfsthread.h
)

set(SOURCES
    qfilemanager.cpp
    qfileremover.cpp
    qfsthread.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
SET (MOC_SOURCES )
SET (UIS )
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

add_library ( ${PROJECT} STATIC ${HEADERS} ${SOURCES} ${MOC_SOURCES})

#add_executable(${PROJECT} ${HEADERS} ${SOURCES} ${MOC_SOURCES})

target_link_libraries(${PROJECT} Qt5::Widgets)

#Uncomment this if you wish to use a shared library
#install(TARGETS ${PROJECT} DESTINATION ${CMAKE_BINARY_DIR}/bin)
