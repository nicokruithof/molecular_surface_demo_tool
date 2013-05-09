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
    osg::ref_ptr<osg::Geometry> result = new osg::Geometry();

    result->setVertexArray(convert_vertices(p));
    result->addPrimitiveSet(convert_indices(p));

    return result.release();
}
osg::Geometry *CgalOsgUtils::convert_mesh_with_normals(const Skin_surface_3 &skin, Polyhedron &p)
{
    osg::Geometry *result = convert_mesh(p);

    result->setNormalArray(convert_normals(skin, p));
    result->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

    return result;
}

std::vector<osg::Geometry *> CgalOsgUtils::convert_skin_mesh(const Skin_surface_3 &skin, Polyhedron &p)
{
    osg::Vec3Array *vs = convert_vertices(p);
    osg::Vec3Array *ns = convert_normals(skin, p);

    osg::ref_ptr<osg::DrawElementsUInt> indices[4];
    for (size_t i=0; i<4; ++i)
        indices[i] = new osg::DrawElementsUInt(GL_TRIANGLES);

    std::map<Polyhedron::Vertex_const_handle, size_t> vertex_index;
    size_t idx = 0;
    for (Polyhedron::Vertex_const_handle vh = p.vertices_begin(); vh != p.vertices_end(); ++vh)
    {
        vertex_index[vh] = idx++;
    }

    typedef typename Polyhedron::Halfedge_around_facet_const_circulator HFC;
    for (Polyhedron::Facet_const_iterator fh = p.facets_begin(); fh != p.facets_end(); ++fh)
    {
        size_t dim = fh->tmc_ch->info().first.dimension();
        HFC hfc = fh->facet_begin();

        for (int i=0; i<3; ++i)
        {
            indices[dim]->push_back(vertex_index[hfc->vertex()]);
            ++hfc;
        }
    }

    std::vector<osg::Geometry *> result;
    for (size_t i=0; i<4; ++i) {
        osg::Geometry *geometry = new osg::Geometry();

        geometry->setVertexArray(vs);
        geometry->setNormalArray(ns);
        geometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
        geometry->addPrimitiveSet(indices[i]);

        result.push_back(geometry);
    }
    return result;
}

osg::Vec3Array *CgalOsgUtils::convert_vertices(const Polyhedron &p)
{
    osg::ref_ptr<osg::Vec3Array> result = new osg::Vec3Array();
    result->reserve(p.size_of_vertices());

    for (Polyhedron::Vertex_const_handle vh = p.vertices_begin(); vh != p.vertices_end(); ++vh)
    {
        result->push_back(conv(vh->point()));
    }

    return result.release();
}
osg::Vec3Array *CgalOsgUtils::convert_normals(const Skin_surface_3 &skin, Polyhedron &p)
{
    CGAL::Skin_surface_refinement_policy_3<Skin_surface_3, Polyhedron> policy(skin);

    osg::ref_ptr<osg::Vec3Array> result = new osg::Vec3Array();
    result->reserve(p.size_of_vertices());

    for (Polyhedron::Vertex_handle vit = p.vertices_begin(); vit != p.vertices_end(); ++vit)
    {
        Vector n = policy.normal(vit);
        n = n/sqrt(n*n);
        result->push_back(conv(n));
    }

    return result.release();
}
osg::DrawElementsUInt *CgalOsgUtils::convert_indices(const Polyhedron &p)
{
    osg::ref_ptr<osg::DrawElementsUInt> result = new osg::DrawElementsUInt(GL_TRIANGLES);

    std::map<Polyhedron::Vertex_const_handle, size_t> vertex_index;
    size_t idx = 0;
    for (Polyhedron::Vertex_const_handle vh = p.vertices_begin(); vh != p.vertices_end(); ++vh)
    {
        vertex_index[vh] = idx++;
    }

    result->reserve(p.size_of_facets()*3);
    typedef typename Polyhedron::Halfedge_around_facet_const_circulator HFC;
    for (Polyhedron::Facet_const_iterator fh = p.facets_begin(); fh != p.facets_end(); ++fh)
    {
        HFC hfc = fh->facet_begin();

        for (int i=0; i<3; ++i)
        {
            result->push_back(vertex_index[hfc->vertex()]);
            ++hfc;
        }
    }

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
