#ifndef OSGHELPER_H
#define OSGHELPER_H

#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/Viewer>
#include <osg/Camera>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/Material>
#include <osgGA/EventQueue>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>

using namespace osg;

ref_ptr<Group> createScene();

ref_ptr<Group> readSceneFile(std::string filePath);
#endif // OSGHELPER_H
