#include "qtosgwidget.h"
#include "osghelper.h"
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>

QtOSGWidget::QtOSGWidget(qreal scaleX, qreal scaleY, QWidget* parent)
      : QOpenGLWidget(parent)
        , _mGraphicsWindow(new osgViewer::GraphicsWindowEmbedded( this->x(), this->y(),
                                                                 this->width(), this->height() ) )
        , _mViewer(new osgViewer::Viewer)
      , m_scaleX(scaleX)
      , m_scaleY(scaleY)
      {

        try{
//        osg::setNotifyLevel(osg::WARN);
//      osg::setNotifyHandler(new OsgLogger("mylog.txt"));
            //initialize camera
        osg::ref_ptr<osg::Camera> camera = new osg::Camera;
        camera->setViewport( this->x(), this->y(), this->width(), this->height() );
        camera->setClearColor( osg::Vec4( 0.9f, 0.9f, 1.f, 1.f ) );
        float aspectRatio = static_cast<float>( this->width()) / static_cast<float>( this->height() );
        camera->setProjectionMatrixAsPerspective( 30.f, aspectRatio, 1.f, 1000.f );
        camera->setGraphicsContext( _mGraphicsWindow );
        _mViewer->setCamera(camera);


        
        //intitalize hud
        osg::ref_ptr<osg::Camera> hud_camera = new osg::Camera;
        hud_camera->setProjectionMatrix(osg::Matrix::ortho2D(0,1280,0,1024));// set the view matrix
        hud_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        hud_camera->setViewMatrix(osg::Matrix::identity());// only clear the depth buffer
        hud_camera->setClearMask(GL_DEPTH_BUFFER_BIT);// draw subgraph after main camera view.
        hud_camera->setRenderOrder(osg::Camera::POST_RENDER);

        hud_camera->setGraphicsContext(_mGraphicsWindow);
        hud_camera->setViewport(this->x(), this->y(),this->width(), this->height());

        _mViewer->addSlave(hud_camera, false);
        //trackball navigation
        osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator;
        manipulator->setAllowThrow( false );
        this->setMouseTracking(true);
        _mViewer->setCameraManipulator(manipulator);

        
                osg::Vec3d eye( 60.0, 40.0, 60.0 );
        osg::Vec3d center( 0.0, 0.0, 0.0 );
        osg::Vec3d up( 0.0, 1.0, 0.0 );

        manipulator->setHomePosition(eye,center,up);
        
        osg::ref_ptr<osg::Group> root = new osg::Group;
       // root->addChild(hud_camera);
        _mViewer->setSceneData(root);
        _mViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

        _mViewer->realize();
        }
        catch(std::exception e)
        {
            std::cout<<e.what()<<std::endl;
        }

      }

void QtOSGWidget::setScene(osg::ref_ptr<osg::Group> root)
{
    _mViewer->setSceneData(root);
}

osg::ref_ptr<osg::Group> QtOSGWidget::getScene()
{
   osg::ref_ptr<osg::Group> root = dynamic_cast<osg::Group*>(_mViewer->getSceneData());
   return root;
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

      //basic shader setup
      // disabled shader as of now, but in essence this is how it will work
      // osg::ref_ptr<osg::StateSet> stateSet = this->getScene()->getOrCreateStateSet();
//       osg::ref_ptr<osg::Program> program = new osg::Program;

//       osg::ref_ptr<osg::Shader> vertShader = new osg::Shader(osg::Shader::VERTEX);
//       if (!vertShader->loadShaderSourceFromFile("../shaders/myShader.vert"))
//           std::cerr << "Could not read VERTEX shader from file" << std::endl;
//       program->addShader(vertShader);

//        osg::ref_ptr<osg::Shader> geomShader = new osg::Shader(osg::Shader::GEOMETRY);
//       if (!geomShader->loadShaderSourceFromFile("../shaders/myShader.geom"))
//           std::cerr << "Could not read GEOM shader from file" << std::endl;
//       program->addShader(geomShader);

//       osg::ref_ptr<osg::Shader> fragShader = new osg::Shader(osg::Shader::FRAGMENT);
//       if (!fragShader->loadShaderSourceFromFile("../shaders/myShader.frag"))
//           std::cerr << "Could not read FRAGMENT shader from file" << std::endl;
//       program->addShader(fragShader);

//       stateSet->setAttributeAndModes(program.get(), osg::StateAttribute::ON);
//       stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);

//       osg::Uniform* modelViewProjectionMatrix = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "osg_ModelViewProjectionMatrix");
//       modelViewProjectionMatrix->setUpdateCallback(new ModelViewProjectionMatrixCallback(_mViewer->getCamera()));
//       stateSet->addUniform(modelViewProjectionMatrix);
  }

  /*void QtOSGWidget::addHud(osg::Group hud)
  {
      hud_camera->addChild(hud);
      hud_camera->setGraphicsContext(_mGraphicsWindow);
      hud_camera->setViewport(this->x(), this->y(),this->width(), this->height());
  }*/
  void QtOSGWidget::mouseMoveEvent(QMouseEvent* event)
  {
      this->getEventQueue()->mouseMotion(event->x()*m_scaleX, event->y()*m_scaleY);
      osg::Matrixd viewMatrix = _mViewer->getCamera()->getViewMatrix();
      osg::Vec3d center;
      osg::Vec3d eye;
      osg::Vec3d up;
      viewMatrix.getLookAt(eye,center,up);
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
    osg::Matrixd viewMatrix = _mViewer->getCamera()->getViewMatrix();
    osg::Vec3d center;
    osg::Vec3d eye;
    osg::Vec3d up;
    viewMatrix.getLookAt(eye,center,up);
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

  void QtOSGWidget::openScene(std::string path)
  {

      if(boost::ends_with(path,".osg"))
      {
        osg::ref_ptr<osg::Node> loadedModel = osgDB::readRefNodeFile(path);
        if(loadedModel == NULL)
          {
             std::cout<<"failed to load model";
             throw std::exception();
          }

          osg::ref_ptr<osg::Group> root = new osg::Group();
          root->addChild(loadedModel);

          _mViewer->setSceneData(root);
      }else if(boost::ends_with(path,".step"))
      {
          osg::ref_ptr<osg::Geode> loadedModel = readStepFile(path);
          if(loadedModel == NULL)
          {
             std::cout<<"failed to load model";
             throw std::exception();
           }
          osg::ref_ptr<osg::Group> root = new osg::Group();
           root->addChild(loadedModel);

          _mViewer->setSceneData(root);
      }

  }

  void QtOSGWidget::saveScene(std::string path)
  {
      osg::ref_ptr<osg::Node> root = _mViewer->getSceneData();
      osgDB::writeNodeFile(*root,path);

  }
