#ifndef OSG_OSG_UTILS_H_
#define OSG_OSG_UTILS_H_

#include <osg/ShapeDrawable>

class OsgUtils
{
public:
    OsgUtils();
    virtual ~OsgUtils();

    static osg::Drawable *create_sphere(const osg::Vec3f &pos, float radius);
};

#endif /* OSG_OSG_UTILS_H_ */
