# This file is part of LyX, the document processor.
# Licence details can be found in the file COPYING.
#
# Copyright (c) 2006-2011, Peter K�mmel, <syntheticpp@gmx.net>
#

project(mathed)

file(GLOB mathed_sources ${TOP_SRC_DIR}/src/mathed/${LYX_CPP_FILES})
file(GLOB mathed_headers ${TOP_SRC_DIR}/src/mathed/${LYX_HPP_FILES})

list(REMOVE_ITEM mathed_sources
	${TOP_SRC_DIR}/src/mathed/InsetMathXYArrow.cpp)

lyx_add_msvc_pch(mathed)

include_directories(${TOP_SRC_DIR}/src/mathed)
include_directories(${TOP_SRC_DIR}/src/common)
INCLUDE_DIRECTORIES(${QT_INCLUDES}
        ${CMAKE_CURRENT_BINARY_DIR})

if(NOT LYX_MERGE_FILES)
	add_library(mathed ${library_type} ${mathed_sources} ${mathed_headers})
else()
	lyx_const_touched_files(_allinone  mathed_sources)
	add_library(mathed ${library_type} ${_allinone_files})
endif()
target_link_libraries(mathed
        ${QT_QTCORE_LIBRARY}
        ${QT_QTGUI_LIBRARY}
)
qt_use_modules(mathed Core Gui Widgets Concurrent)
set_target_properties(mathed PROPERTIES FOLDER "applications/LyX")
#MAXVAS
if (${UNIX})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif (${UNIX})

lyx_add_gcc_pch(mathed)

project_source_group("${GROUP_CODE}" mathed_sources mathed_headers)

