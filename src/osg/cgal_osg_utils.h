#ifndef OSG_CGAL_OSG_UTILS_H_
#define OSG_CGAL_OSG_UTILS_H_

#include <osg/Geometry>
#include <model/types.h>

class CgalOsgUtils
{
public:
    CgalOsgUtils();
    virtual ~CgalOsgUtils();

    static osg::Geometry *convert_mesh(const Polyhedron &p);
    static osg::Geometry *convert_mesh_with_normals(const Skin_surface_3 &skin, Polyhedron &p);

private:
    static osg::Vec3f conv(const Bare_point &p);
    static osg::Vec3f conv(const Vector &p);
};

#endif /* OSG_CGAL_OSG_UTILS_H_ */
