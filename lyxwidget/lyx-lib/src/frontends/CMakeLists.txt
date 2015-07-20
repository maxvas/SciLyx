# This file is part of LyX, the document processor.
# Licence details can be found in the file COPYING.
#
# Copyright (c) 2006-2011 Peter K�mmel, <syntheticpp@gmx.net>
#

project(frontends)

include_directories(${TOP_SRC_DIR}/src/frontends
    ${QT_INCLUDES})

add_subdirectory(qt4) 

file(GLOB frontends_sources ${TOP_SRC_DIR}/src/frontends/${LYX_CPP_FILES})
file(GLOB frontends_headers ${TOP_SRC_DIR}/src/frontends/${LYX_HPP_FILES})

lyx_automoc(${frontends_sources})

if(NOT LYX_MERGE_FILES)
	add_library(frontends ${library_type} ${frontends_sources} ${frontends_headers})
else()
	lyx_const_touched_files(_allinone  frontends_sources)
	add_library(frontends ${library_type} ${_allinone_files})
endif()
set_target_properties(frontends PROPERTIES FOLDER "applications/LyX")
#MAXVAS
if (${UNIX})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif (${UNIX})

if(Qt5Core_FOUND AND APPLE)
       qt_use_modules(frontends Core Gui Widgets Concurrent MacExtras)
else()
       qt_use_modules(frontends Core Gui Widgets Concurrent)
endif()
target_link_libraries(frontends ${Lyx_Boost_Libraries} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

project_source_group("${GROUP_CODE}" frontends_sources frontends_headers)

add_subdirectory(tests)
