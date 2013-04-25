#include "osg/cgal_osg_utils.h"

#include <queue>

#include <boost/array.hpp>
#include <boost/foreach.hpp>

#include <osg/Geometry>

CgalOsgUtils::CgalOsgUtils()
{
}

CgalOsgUtils::~CgalOsgUtils()
{
}

osg::Geometry *CgalOsgUtils::convert_mesh(const Polyhedron &p)
{
    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    //osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    osg::ref_ptr<osg::DrawElementsUShort> indices = new osg::DrawElementsUShort(GL_TRIANGLES);

    vertices->reserve(p.size_of_vertices());

    std::map<Polyhedron::Vertex_const_handle, size_t> vertex_index;
    size_t idx = 0;
    for (Polyhedron::Vertex_const_handle vh = p.vertices_begin(); vh != p.vertices_end(); ++vh)
    {
        vertex_index[vh] = idx;
        vertices->push_back(osg::Vec3f(vh->point().x(), vh->point().y(), vh->point().z()));
        ++idx;
    }

    indices->reserve(p.size_of_facets()*3);
    typedef typename Polyhedron::Halfedge_around_facet_const_circulator HFC;
    for (Polyhedron::Facet_const_iterator fh = p.facets_begin(); fh != p.facets_end(); ++fh)
    {
        HFC hfc = fh->facet_begin();

        for (int i=0; i<3; ++i)
        {
            indices->push_back(vertex_index[hfc->vertex()]);
            ++hfc;
        }
    }


    osg::ref_ptr<osg::Geometry> result = new osg::Geometry();
    result->setVertexArray(vertices);
    result->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
    result->addPrimitiveSet(indices);

    return result.release();
}
osg::Geometry *CgalOsgUtils::convert_mesh_with_normals(const Skin_surface_3 &skin, Polyhedron &p)
{
    CGAL::Skin_surface_refinement_policy_3<Skin_surface_3, Polyhedron> policy(skin);

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    osg::ref_ptr<osg::DrawElementsUShort> indices = new osg::DrawElementsUShort(GL_TRIANGLES);

    vertices->reserve(p.size_of_vertices());
    normals->reserve(p.size_of_vertices());

    std::map<Polyhedron::Vertex_const_handle, size_t> vertex_index;
    size_t idx = 0;
    for (Polyhedron::Vertex_handle vit = p.vertices_begin(); vit != p.vertices_end(); ++vit)
    {
        vertex_index[vit] = idx;
        vertices->push_back(conv(vit->point()));

        Vector n = policy.normal(vit);
        n = n/sqrt(n*n);
        normals->push_back(conv(n));
        ++idx;
    }

    indices->reserve(p.size_of_facets()*3);
    typedef typename Polyhedron::Halfedge_around_facet_const_circulator HFC;
    for (Polyhedron::Facet_const_iterator fh = p.facets_begin(); fh != p.facets_end(); ++fh)
    {
        HFC hfc = fh->facet_begin();

        for (int i=0; i<3; ++i)
        {
            indices->push_back(vertex_index[hfc->vertex()]);
            ++hfc;
        }
    }


    osg::ref_ptr<osg::Geometry> result = new osg::Geometry();
    result->setVertexArray(vertices);
    result->setNormalArray(normals);
    result->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
    result->addPrimitiveSet(indices);

    return result.release();
}

osg::Vec3f CgalOsgUtils::conv(const Bare_point &p)
{
    return osg::Vec3f(p.x(), p.y(), p.z());
}
osg::Vec3f CgalOsgUtils::conv(const Vector &p)
{
    return osg::Vec3f(p.x(), p.y(), p.z());
}
