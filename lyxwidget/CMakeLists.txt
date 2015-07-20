SET(CMAKE_SHARED_LIBRARY_PREFIX "")
SET (PROJECT lyxwidget)

SET (SOURCES
    lyxwidget.cpp
)

SET (HEADERS
    lyxwidget.h
)

SET (UI
    lyxwidget.ui
)

project (${PROJECT})

find_package( Qt5 REQUIRED COMPONENTS Core Gui Widgets)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lyx-lib/boost)

SET (MOC_SOURCES )
SET (UIS )
qt5_wrap_cpp (MOC_SOURCES ${HEADERS})
qt5_wrap_ui (UIS ${UI})

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

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lyx-lib/src)

add_library ( ${PROJECT} SHARED ${HEADERS} ${SOURCES} ${MOC_SOURCES} ${UIS})

add_subdirectory(lyx-lib ${CMAKE_CURRENT_BINARY_DIR}/lyx-lib)
target_link_libraries(${PROJECT_NAME} LyX_testlib)

#install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}${CMAKE_SHARED_MODULE_SUFFIX} DESTINATION ${CMAKE_BINARY_DIR}/bin )
install(TARGETS ${PROJECT} DESTINATION ${CMAKE_BINARY_DIR}/bin)
qt5_use_modules(${PROJECT} Core Widgets)

install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lyx-lib/lib DESTINATION ${CMAKE_BINARY_DIR}/bin/lyx-system )
#install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lyx-lib/lib DESTINATION ${CMAKE_BINARY_DIR}/lyx-system )

#Uncomment this if you wish to use a shared library
#install(TARGETS ${PROJECT} DESTINATION ${CMAKE_BINARY_DIR}/bin)
if(WIN32)
    #Install libiconv2
    install (FILES "${GNUWIN32_DIR}/bin/libiconv2.dll" DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif()
