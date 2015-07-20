# This file is part of LyX, the document processor.
# Licence details can be found in the file COPYING.
#
# Copyright (c) 2006-2011 Peter K�mmel, <syntheticpp@gmx.net>
#

project(insets)

file(GLOB insets_sources ${TOP_SRC_DIR}/src/insets/${LYX_CPP_FILES})
file(GLOB insets_headers ${TOP_SRC_DIR}/src/insets/${LYX_HPP_FILES})

list(REMOVE_ITEM insets_sources ${TOP_SRC_DIR}/src/insets/InsetTheorem.cpp)

lyx_add_msvc_pch(insets)
include_directories(${TOP_SRC_DIR}/src/insets ${QT_INCLUDES})
include_directories(${TOP_SRC_DIR}/src/common)


if(NOT LYX_MERGE_FILES)
        add_library(insets ${library_type} ${insets_sources} ${insets_headers})
else()
	lyx_const_touched_files(_allinone  insets_sources)
	add_library(insets ${library_type} ${_allinone_files})
endif()
lyx_automoc(${insets_sources} ${insets_headers})
set_target_properties(insets PROPERTIES FOLDER "applications/LyX")
#MAXVAS
if (${UNIX})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif (${UNIX})
qt_use_modules(insets Core Gui Widgets)
target_link_libraries(insets ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

lyx_add_gcc_pch(insets)

project_source_group("${GROUP_CODE}" insets_sources insets_headers)

