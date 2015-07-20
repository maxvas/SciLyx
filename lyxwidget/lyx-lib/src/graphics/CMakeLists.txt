# This file is part of LyX, the document processor.
# Licence details can be found in the file COPYING.
#
# Copyright (c) 2006-2011 Peter K�mmel, <syntheticpp@gmx.net>
#

project(graphics)

file(GLOB graphics_sources ${TOP_SRC_DIR}/src/graphics/${LYX_CPP_FILES})
file(GLOB graphics_headers ${TOP_SRC_DIR}/src/graphics/${LYX_HPP_FILES})

lyx_add_msvc_pch(graphics)
include_directories(${TOP_SRC_DIR}/src/graphics ${QT_INCLUDES})

if(NOT LYX_MERGE_FILES)
	add_library(graphics ${library_type} ${graphics_sources} ${graphics_headers})
else()
	lyx_const_touched_files(_allinone  graphics_sources)
	add_library(graphics ${library_type} ${_allinone_files})
endif()
lyx_automoc(${graphics_sources} ${graphics_headers})
set_target_properties(graphics PROPERTIES FOLDER "applications/LyX")
#MAXVAS
if (${UNIX})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif (${UNIX})
qt_use_modules(graphics Core Gui)
target_link_libraries(graphics ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

lyx_add_gcc_pch(graphics)

project_source_group("${GROUP_CODE}" graphics_sources graphics_headers)

