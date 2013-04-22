#include <osg/screencapture.h>

#include <osg/GraphicsContext>
#include <osgDB/WriteFile>

ScreenCapture::ScreenCapture(const std::string &filename) : m_filename(filename) {}

void ScreenCapture::operator()( osg::RenderInfo& ri ) const
{
	std::cout << __FUNCTION__ << std::endl;
	const osg::GraphicsContext::Traits *traits = ri.getCurrentCamera()->getGraphicsContext()->getTraits();

	std::string fName( m_filename.c_str() );
	osg::ref_ptr<osg::Image> image = new osg::Image;
	image->readPixels( 0, 0, traits->width, traits->height, GL_RGBA, GL_UNSIGNED_BYTE );
	osgDB::writeImageFile( *image, fName );

	ri.getCurrentCamera()->setFinalDrawCallback(NULL);
}
