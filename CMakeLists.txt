cmake_minimum_required(VERSION 3.1.0)

project(Qtworkbench)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)

FIND_PACKAGE( OpenSceneGraph COMPONENTS osgDB osgGA osgViewer osg)

add_executable(Qtworkbench
    mainwindow.ui
    qtosgwidget.cpp
    mainwindow.cpp
    main.cpp
)

target_link_libraries(Qtworkbench Qt5::Widgets ${OPENSCENEGRAPH_LIBRARIES})
