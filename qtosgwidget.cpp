#include "qtosgwidget.h"



QtOSGWidget::QtOSGWidget(qreal scaleX, qreal scaleY, QWidget* parent)
      : QOpenGLWidget(parent)
        , _mGraphicsWindow(new osgViewer::GraphicsWindowEmbedded( this->x(), this->y(),
                                                                 this->width(), this->height() ) )
        , _mViewer(new osgViewer::Viewer)
      , m_scaleX(scaleX)
      , m_scaleY(scaleY)
      {

        try{

            //initialize camera
        osg::ref_ptr<osg::Camera> camera = new osg::Camera;
        camera->setViewport( this->x(), this->y(), this->width(), this->height() );
        camera->setClearColor( osg::Vec4( 0.9f, 0.9f, 1.f, 1.f ) );
        float aspectRatio = static_cast<float>( this->width()) / static_cast<float>( this->height() );
        camera->setProjectionMatrixAsPerspective( 30.f, aspectRatio, 1.f, 1000.f );
        camera->setGraphicsContext( _mGraphicsWindow );
        _mViewer->setCamera(camera);

        //trackball navigation
        osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator;
        manipulator->setAllowThrow( false );
        this->setMouseTracking(true);
        _mViewer->setCameraManipulator(manipulator);

        osg::ref_ptr<osg::Cylinder> cylinder    = new osg::Cylinder( osg::Vec3( 0.f, 0.f, 0.f ), 0.25f, 0.5f );
        osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable( cylinder );
        sd->setColor( osg::Vec4( 0.8f, 0.5f, 0.2f, 1.f ) );
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        geode->addDrawable(sd);
        osg::ref_ptr<osg::Group> root = new osg::Group();

        root->addChild(geode);

        _mViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);


        _mViewer->setSceneData(root);
        _mViewer->realize();
        }
        catch(std::exception e)
        {
            std::cout<<e.what()<<std::endl;
        }

      }


  void QtOSGWidget::setScale(qreal X, qreal Y)
  {
      m_scaleX = X;
      m_scaleY = Y;
      this->resizeGL(this->width(), this->height());
  }


  void QtOSGWidget::paintGL() {
    _mViewer->frame();
  }

  void QtOSGWidget::resizeGL( int width, int height )
  {
      this->getEventQueue()->windowResize(this->x()*m_scaleX, this->y() * m_scaleY, width*m_scaleX, height*m_scaleY);
      _mGraphicsWindow->resized(this->x()*m_scaleX, this->y() * m_scaleY, width*m_scaleX, height*m_scaleY);
      osg::ref_ptr<osg::Camera> camera = _mViewer->getCamera();
      camera->setViewport(0, 0, this->width()*m_scaleX, this->height()* m_scaleY);
  }

  void QtOSGWidget::initializeGL(){
      osg::ref_ptr<osg::Group> geode = dynamic_cast<osg::Group*>(_mViewer->getSceneData());
     osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();
      osg::ref_ptr<osg::Material> material = new osg::Material;
      material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
      stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
      stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
  }

  void QtOSGWidget::mouseMoveEvent(QMouseEvent* event)
  {
      this->getEventQueue()->mouseMotion(event->x()*m_scaleX, event->y()*m_scaleY);
      osg::Matrixd viewMatirx = _mViewer->getCamera()->getViewMatrix();
      osg::Vec3d center;
      osg::Vec3d eye;
      osg::Vec3d up;
      viewMatirx.getLookAt(eye,center,up);
      std::vector<double> values;

      values.push_back(eye.x());
      values.push_back(eye.y());
      values.push_back(eye.z());

      values.push_back(center.x());
      values.push_back(center.y());
      values.push_back(center.z());

      values.push_back(up.x());
      values.push_back(up.y());
      values.push_back(up.z());

      emit orientationChanged(values);
  }

  void QtOSGWidget::mousePressEvent(QMouseEvent* event)
  {
      unsigned int button = 0;
      switch (event->button()){
      case Qt::LeftButton:
          button = 1;
          break;
      case Qt::MiddleButton:
          button = 2;
          break;
      case Qt::RightButton:
          button = 3;
          break;
      default:
          break;
      }
      this->getEventQueue()->mouseButtonPress(event->x()*m_scaleX, event->y()*m_scaleY, button);
  }

  void QtOSGWidget::mouseReleaseEvent(QMouseEvent* event)
  {
      unsigned int button = 0;
      switch (event->button()){
      case Qt::LeftButton:
          button = 1;
          break;
      case Qt::MiddleButton:
          button = 2;
          break;
      case Qt::RightButton:
          button = 3;
          break;
      default:
          break;
      }
      this->getEventQueue()->mouseButtonRelease(event->x()*m_scaleX, event->y()*m_scaleY, button);
  }

  void QtOSGWidget::wheelEvent(QWheelEvent* event)
  {
      int delta = event->delta();
      osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?
                  osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;
      this->getEventQueue()->mouseScroll(motion);
    osg::Matrixd viewMatirx = _mViewer->getCamera()->getViewMatrix();
    osg::Vec3d center;
    osg::Vec3d eye;
    osg::Vec3d up;
    viewMatirx.getLookAt(eye,center,up);
    std::vector<double> values;

    values.push_back(eye.x());
    values.push_back(eye.y());
    values.push_back(eye.z());

    values.push_back(center.x());
    values.push_back(center.y());
    values.push_back(center.z());

    values.push_back(up.x());
    values.push_back(up.y());
    values.push_back(up.z());

    emit orientationChanged(values);
  }

  bool QtOSGWidget::event(QEvent* event)
  {
      bool handled = QOpenGLWidget::event(event);
      this->update();
      return handled;
  }

  osgGA::EventQueue* QtOSGWidget::getEventQueue() const {
    osgGA::EventQueue* eventQueue = _mGraphicsWindow->getEventQueue();
    return eventQueue;
  }

  void QtOSGWidget::changedColor(QColor color)
  {
      int r = 0,b = 0,g = 0,a = 255;
     osg::Vec4 vColor;
     color.getRgb(&r,&b,&g,&a);
     vColor[0] = ((float)r)/255.0f;
     vColor[1] = ((float)g)/255.0f;
     vColor[2] = ((float)b)/255.0f;
     vColor[3] = ((float)a)/255.0f;

    _mViewer->getCamera()->setClearColor(vColor);
  }
