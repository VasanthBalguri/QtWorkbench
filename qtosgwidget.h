#ifndef QTOSGWIDGET_H
#define QTOSGWIDGET_H

#include <QApplication>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDesktopWidget>
#include <QScreen>
#include <QtGlobal>
#include <QWindow>

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

#include<iostream>
#include<vector>

class QtOSGWidget : public QOpenGLWidget
{
private:

  osgGA::EventQueue* getEventQueue() const;

  osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _mGraphicsWindow;
  osg::ref_ptr<osgViewer::Viewer> _mViewer;
  qreal m_scaleX, m_scaleY;

public:
  QtOSGWidget(qreal scaleX = 1, qreal scaleY = 1, QWidget* parent = 0);

  void addScene(osg::ref_ptr<osg::Group> root);
  osg::ref_ptr<osg::Group> getScene();

  virtual ~QtOSGWidget(){}

  void setScale(qreal X, qreal Y);

  Q_OBJECT
public slots:
    void changedColor(QColor color);
    void openScene(std::string path);
signals:
    void orientationChanged(std::vector<double>);


protected:

  virtual void paintGL();

  virtual void resizeGL( int width, int height );

  virtual void initializeGL();

  virtual void mouseMoveEvent(QMouseEvent* event);

  virtual void mousePressEvent(QMouseEvent* event);

  virtual void mouseReleaseEvent(QMouseEvent* event);


  virtual void wheelEvent(QWheelEvent* event);

  virtual bool event(QEvent* event);

};
#endif // QTOSGWIDGET_H
