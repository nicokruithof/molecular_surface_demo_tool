#include "osg/osg_utils.h"

#include <boost/foreach.hpp>

#include <osg/Geometry>

OsgUtils::OsgUtils()
{
}

OsgUtils::~OsgUtils()
{
}

osg::Drawable *OsgUtils::create_sphere(const osg::Vec3f &pos, float radius)
{
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    osg::ref_ptr<osg::DrawElementsUShort> indices = new osg::DrawElementsUShort(GL_TRIANGLES);

    float d = sqrt(0.5f);
    normals->reserve(4);
    normals->push_back(osg::Vec3f(d, d, d));
    normals->push_back(osg::Vec3f(-d, -d, d));
    normals->push_back(osg::Vec3f(d, -d, -d));
    normals->push_back(osg::Vec3f(-d, d, -d));

    vertices->reserve(normals->size());

    BOOST_FOREACH(osg::Vec3f &n, *normals)
    {
        vertices->push_back(pos + n * radius);
    }

    indices->push_back(0);
    indices->push_back(1);
    indices->push_back(2);

    indices->push_back(1);
    indices->push_back(0);
    indices->push_back(3);

    indices->push_back(2);
    indices->push_back(3);
    indices->push_back(0);

    indices->push_back(3);
    indices->push_back(2);
    indices->push_back(1);

    osg::ref_ptr<osg::Geometry> result = new osg::Geometry();
    result->setVertexArray(vertices);
    result->setNormalArray(normals);
    result->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
    result->addPrimitiveSet(indices);

    return result.release();
}
