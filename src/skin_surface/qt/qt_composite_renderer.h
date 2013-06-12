#ifndef QT_COMPOSITE_RENDERER_H
#define QT_COMPOSITE_RENDERER_H

#include <QtGui/QWidget>
#include <osgViewer/CompositeViewer>

#include <QtCore/QTimer>

class QtCompositeRenderer : public QWidget
{
  Q_OBJECT;

public:
  QtCompositeRenderer(osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::CompositeViewer::SingleThreaded);

  static osg::ref_ptr<osgViewer::CompositeViewer> composite_viewer();
  static void add_view(osgViewer::View *view);

private slots:
  virtual void update();

protected:
  QTimer _timer;

  static QtCompositeRenderer *m_singleton;
  osg::ref_ptr<osgViewer::CompositeViewer> m_viewer;
};

#endif // QT_COMPOSITE_RENDERER_H
