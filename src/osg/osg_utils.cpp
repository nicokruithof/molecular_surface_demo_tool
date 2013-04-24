#include "osg/osg_utils.h"

#include <queue>

#include <boost/array.hpp>
#include <boost/foreach.hpp>

#include <osg/Geometry>

OsgUtils::OsgUtils()
{
}

OsgUtils::~OsgUtils()
{
}

struct Triangle {
    Triangle(unsigned short i,unsigned short j,unsigned short k) { idx[0] = i; idx[1] = j; idx[2] = k; }
    unsigned short idx[3];
};

osg::Drawable *OsgUtils::create_sphere(const osg::Vec3f &pos, float radius, size_t subdiv)
{
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    osg::ref_ptr<osg::DrawElementsUShort> indices = new osg::DrawElementsUShort(GL_TRIANGLES);

    float d = sqrt(1/3.0f);
    normals->reserve(4);
    normals->push_back(osg::Vec3f(d, d, d));
    normals->push_back(osg::Vec3f(-d, -d, d));
    normals->push_back(osg::Vec3f(d, -d, -d));
    normals->push_back(osg::Vec3f(-d, d, -d));

    typedef std::pair<size_t, Triangle >    SubdivTriangle;

    std::queue<SubdivTriangle> triangles;
    triangles.push(SubdivTriangle(subdiv, Triangle(0,1,2)));
    triangles.push(SubdivTriangle(subdiv, Triangle(1,0,3)));
    triangles.push(SubdivTriangle(subdiv, Triangle(2,3,0)));
    triangles.push(SubdivTriangle(subdiv, Triangle(3,2,1)));
    while (!triangles.empty())
    {
        const size_t subdiv = triangles.front().first;
        const Triangle &triangle = triangles.front().second;
        if (subdiv == 0)
        {
            for (size_t i=0; i<3; ++i)
                indices->push_back(triangle.idx[i]);
        }
        else
        {
            osg::Vec3f n;

            unsigned short i01 = normals->size();
            unsigned short i02 = i01+1;
            unsigned short i12 = i02+1;

            n = (((*normals)[triangle.idx[0]]+(*normals)[triangle.idx[1]])/2.0);
            n.normalize();
            normals->push_back(n);
            n = (((*normals)[triangle.idx[0]]+(*normals)[triangle.idx[2]])/2.0);
            n.normalize();
            normals->push_back(n);
            n = (((*normals)[triangle.idx[1]]+(*normals)[triangle.idx[2]])/2.0);
            n.normalize();
            normals->push_back(n);

            triangles.push(SubdivTriangle(subdiv-1, Triangle(i12, i02, i01)));
            triangles.push(SubdivTriangle(subdiv-1, Triangle(triangle.idx[0], i01, i02)));
            triangles.push(SubdivTriangle(subdiv-1, Triangle(i01, triangle.idx[1], i12)));
            triangles.push(SubdivTriangle(subdiv-1, Triangle(i02, i12, triangle.idx[2])));
        }
        triangles.pop();
    }
//    indices->push_back(0);
//    indices->push_back(1);
//    indices->push_back(2);
//
//    indices->push_back(1);
//    indices->push_back(0);
//    indices->push_back(3);
//
//    indices->push_back(2);
//    indices->push_back(3);
//    indices->push_back(0);
//
//    indices->push_back(3);
//    indices->push_back(2);
//    indices->push_back(1);

    vertices->reserve(normals->size());
    BOOST_FOREACH(osg::Vec3f &n, *normals)
        vertices->push_back(pos + n * radius);


    osg::ref_ptr<osg::Geometry> result = new osg::Geometry();
    result->setVertexArray(vertices);
    result->setNormalArray(normals);
    result->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
    result->addPrimitiveSet(indices);

    return result.release();
}
