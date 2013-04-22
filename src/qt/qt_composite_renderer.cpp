#include "./qt_composite_renderer.h"

#include <osgViewer/CompositeViewer>

QtCompositeRenderer *QtCompositeRenderer::m_singleton=NULL;

QtCompositeRenderer::QtCompositeRenderer(osgViewer::ViewerBase::ThreadingModel threadingModel)
: m_viewer(new osgViewer::CompositeViewer)
{
	m_viewer->setThreadingModel(threadingModel);

	connect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
	_timer.start( 10 );
}

osg::ref_ptr<osgViewer::CompositeViewer> QtCompositeRenderer::composite_viewer() {
	if (m_singleton == NULL) {
		m_singleton = new QtCompositeRenderer();
	}
	return m_singleton->m_viewer;
}

void QtCompositeRenderer::add_view(osgViewer::View *view) {
  composite_viewer()->addView(view);
}

void QtCompositeRenderer::update() {
  m_viewer->frame();
}



