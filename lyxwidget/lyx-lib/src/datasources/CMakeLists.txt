cmake_minimum_required(VERSION 2.8.11)
SET (PROJECT datasources)

add_subdirectory(csv)

project(${PROJECT})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)

add_library(${PROJECT} STATIC AbstractDataSelector.h DataSourceManager.h DataSourceManager.cpp LapTable.h LapTable.cpp)
if (${UNIX})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif (${UNIX})
target_link_libraries(${PROJECT} Qt5::Widgets)


