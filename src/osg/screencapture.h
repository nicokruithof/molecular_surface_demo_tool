#ifndef SCREEN_CAPTURE_H
#define SCREEN_CAPTURE_H

#include <string>
#include <osg/Camera>

class ScreenCapture : public osg::Camera::DrawCallback
{
public:
	ScreenCapture(const std::string &filename);

    virtual void operator()( osg::RenderInfo& ri ) const;

private:
    std::string m_filename;
};

#endif // SCREEN_CAPTURE_H
