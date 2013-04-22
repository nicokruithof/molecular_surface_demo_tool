#include <qt/open_scene_graph_widget.h>

#include <QtGui/QGridLayout>

#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>

#include <osgDB/ReadFile>

#include <osgQt/GraphicsWindowQt>

#include <qt/qt_composite_renderer.h>

OpenSceneGraphWidget::OpenSceneGraphWidget(QWidget * parent, Qt::WindowFlags f)
: QWidget(parent, f)
, m_view(NULL)
{
  QWidget* ogl_widget = addViewWidget( createCamera(0,0,100,100), NULL );

  QGridLayout* grid = new QGridLayout;
  grid->setSpacing (0);
  grid->setContentsMargins (0, 0, 0, 0);
  grid->addWidget( ogl_widget, 0, 0 );
  setLayout( grid );
}

void OpenSceneGraphWidget::set_scene(osg::Node *scene) {
  m_view->setSceneData(scene);
}
    
QWidget* OpenSceneGraphWidget::addViewWidget( osg::Camera* camera, osg::Node* scene ) {
  m_view = new osgViewer::View;
  m_view->setCamera( camera );
  QtCompositeRenderer::add_view( m_view );
        
  m_view->setSceneData( scene );
  m_view->addEventHandler( new osgViewer::StatsHandler );
  osgGA::TrackballManipulator *manipulator = new osgGA::TrackballManipulator();
  manipulator->setVerticalAxisFixed(true);
  m_view->setCameraManipulator( manipulator );
        
  osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( camera->getGraphicsContext() );
  return gw ? gw->getGLWidget() : NULL;
}
    
osg::Camera* OpenSceneGraphWidget::createCamera( int x, int y, int w, int h, const std::string& name, bool windowDecoration) {
  osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
  osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
  traits->windowName = name;
  traits->windowDecoration = windowDecoration;
  traits->x = x;
  traits->y = y;
  traits->width = w;
  traits->height = h;
  traits->doubleBuffer = true;
  traits->alpha = ds->getMinimumNumAlphaBits();
  traits->stencil = ds->getMinimumNumStencilBits();
  traits->sampleBuffers = ds->getMultiSamples();
  traits->samples = ds->getNumMultiSamples();
        
  osg::ref_ptr<osg::Camera> camera = new osg::Camera;
  camera->setGraphicsContext( new osgQt::GraphicsWindowQt(traits.get()) );
        
  camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
  camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
  camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );
  return camera.release();
}
