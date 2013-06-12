#ifndef OPEN_SCENE_GRAPH_WIDGET_H
#define OPEN_SCENE_GRAPH_WIDGET_H

#include <QtGui/QWidget>
#include <osgViewer/CompositeViewer>

class OpenSceneGraphWidget : public QWidget {
  Q_OBJECT

public:
  OpenSceneGraphWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);

  void set_scene(osg::Node *scene);

private:
  QWidget* addViewWidget( osg::Camera* camera, osg::Node* scene );
    
  osg::Camera* createCamera( int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false );


  osgViewer::View* m_view;
};

#endif // OPEN_SCENE_GRAPH_WIDGET_H
