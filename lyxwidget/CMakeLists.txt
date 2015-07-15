SET(CMAKE_SHARED_LIBRARY_PREFIX "")
SET (PROJECT lyxwidget)

SET (SOURCES
    lyxwidget.cpp
    configuredialog.cpp
)

SET (HEADERS
    lyxwidget.h
    configuredialog.h
)

SET (UI
    lyxwidget.ui
    configuredialog.ui
)

project (${PROJECT})

find_package( Qt5 REQUIRED COMPONENTS Core Gui Widgets)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lyx-lib/boost)

SET (MOC_SOURCES )
SET (UIS )
qt5_wrap_cpp (MOC_SOURCES ${HEADERS})
qt5_wrap_ui (UIS ${UI})

#cmake_policy(SET CMP0028 OLD)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lyx-lib/src)

set(CMAKE_CXX_FLAGS "-fPIC")

add_library ( ${PROJECT} STATIC ${HEADERS} ${SOURCES} ${MOC_SOURCES} ${UIS})

add_subdirectory(lyx-lib ${CMAKE_CURRENT_BINARY_DIR}/lyx-lib)
target_link_libraries(${PROJECT_NAME} LyX_testlib)

#install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}${CMAKE_SHARED_MODULE_SUFFIX} DESTINATION ${CMAKE_BINARY_DIR}/bin )
qt5_use_modules(${PROJECT} Core Widgets)

install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lyx-lib/lib DESTINATION ${CMAKE_BINARY_DIR}/bin/lyx-system )
#install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lyx-lib/lib DESTINATION ${CMAKE_BINARY_DIR}/lyx-system )

#Uncomment this if you wish to use a shared library
#install(TARGETS ${PROJECT} DESTINATION ${CMAKE_BINARY_DIR}/bin)
if(WIN32)
    #Install libiconv2
    install (FILES "${GNUWIN32_DIR}/bin/libiconv2.dll" DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif()
