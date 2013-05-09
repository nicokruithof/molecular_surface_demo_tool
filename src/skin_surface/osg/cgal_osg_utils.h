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

    // 4 geometries, for each cell type one
    static std::vector<osg::Geometry *> convert_skin_mesh(const Skin_surface_3 &skin, Polyhedron &p);

private:
    static osg::Vec3Array *convert_vertices(const Polyhedron &p);
    static osg::Vec3Array *convert_normals(const Skin_surface_3 &skin, Polyhedron &p);
    static osg::DrawElementsUInt *convert_indices(const Polyhedron &p);

    static osg::Vec3f conv(const Bare_point &p);
    static osg::Vec3f conv(const Vector &p);
};

#endif /* OSG_CGAL_OSG_UTILS_H_ */
