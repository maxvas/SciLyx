project(common)

file(GLOB common_sources ${TOP_SRC_DIR}/src/common/${LYX_CPP_FILES})
file(GLOB common_headers ${TOP_SRC_DIR}/src/common/${LYX_HPP_FILES})
file(GLOB common_moc_files ${TOP_SRC_DIR}/src/common/${LYX_MOC_FILES})

lyx_add_msvc_pch(common)
include_directories(${TOP_SRC_DIR}/src/common ${QT_INCLUDES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

lyx_automoc(${common_sources})

if(NOT LYX_MERGE_FILES)
	add_library(common ${library_type} ${common_sources} ${common_headers} ${common_moc_files})
else()
	lyx_const_touched_files(_allinone  common_sources)
	add_library(common ${library_type} ${_allinone_files})
endif()
set_target_properties(common PROPERTIES FOLDER "applications/LyX")
target_link_libraries(common support)
#MAXVAS
if (${UNIX})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif (${UNIX})

qt_use_modules(common Core Gui Widgets Concurrent)

lyx_add_gcc_pch(common)

project_source_group("${GROUP_CODE}" common_sources common_headers)

