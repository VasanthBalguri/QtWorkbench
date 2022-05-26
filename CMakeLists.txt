cmake_minimum_required(VERSION 3.1.0)

project(Qtworkbench)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)
find_package (OpenCASCADE REQUIRED)
FIND_PACKAGE( OpenSceneGraph COMPONENTS osgDB osgGA osgViewer osg osgUtil osgText)
find_package(Boost REQUIRED)
#include_directories(${OCE_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
include_directories(${OpenCASCADE_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${OPENSCENEGRAPH_INCLUDE_DIRS})

message(STATUS "OSG Incl dirs: ${OPENCASCADE_LIBRARIES} ;${OpenCASCADE_INCLUDE_DIR} ; ${Boost_INCLUDE_DIRS} ; ${OPENSCENEGRAPH_INCLUDE_DIRS} ")

add_executable(Qtworkbench
    mainwindow.ui
    qtosgwidget.cpp
    osghelper.cpp
    mainwindow.cpp
    main.cpp
)

#target_link_libraries(Qtworkbench Qt5::Widgets ${OPENSCENEGRAPH_LIBRARIES} ${OPENCASCADE_LIBRARIES})

target_link_libraries(Qtworkbench Qt5::Widgets ${OPENSCENEGRAPH_LIBRARIES} TKBinL TKBin TKBinTObj TKBinXCAF TKBool TKBO TKBRep TKCAF TKCDF TKernel TKFeat TKFillet TKGeomAlgo TKGeomBase TKG2d TKG3d TKHLR TKIGES TKLCAF TKMath TKMesh TKMeshVS TKOffset TKOpenGl TKPrim TKService TKShHealing TKSTEPAttr TKSTEPBase TKSTEP TKSTEP209 TKSTL TKTObj TKTopAlgo TKVRML TKV3d TKXCAF TKXDEIGES TKXDESTEP TKXMesh TKXmlL TKXml TKXmlTObj TKXmlXCAF TKXSBase)
