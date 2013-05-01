#include "model/model.h"
#include "model/model_data.h"

#include <fstream>

#include <boost/foreach.hpp>

#include <osg/Geode>
#include <osg/CullFace>
#include <osg/Material>

#include <model/extract_balls_from_pdb.h>

#include <osg/osg_utils.h>
#include <osg/cgal_osg_utils.h>

Model::Model()
: m_model_data(new ModelData())
{
}

Model::~Model() {
}

ModelData &Model::data() const
{
    assert(m_model_data != NULL);
    return *m_model_data;
}

osg::ref_ptr<osg::Node> Model::scene() const {
    return data().m_scene;
}

void Model::get_statistics(std::list<Statistic> &stats)
{
    stats.clear();
    Statistic s;

    s.category = "input";
    s.set("# balls", data().m_input_points.data().size());
    stats.push_back(s);
    s.set("shrink_factor", data().m_shrinkfactor.data());
    stats.push_back(s);

    s.category = "regular triangulation";
    s.set("# vertices", data().m_skin_surface.data()->regular().number_of_vertices());
    stats.push_back(s);
    s.set("# edges", data().m_skin_surface.data()->regular().number_of_finite_edges());
    stats.push_back(s);
    s.set("# faces", data().m_skin_surface.data()->regular().number_of_finite_facets());
    stats.push_back(s);
    s.set("# cells", data().m_skin_surface.data()->regular().number_of_finite_cells());
    stats.push_back(s);

    s.category = "triangulated mixed complex";
    s.set("# vertices", data().m_skin_surface.data()->triangulated_mixed_complex().number_of_vertices());
    stats.push_back(s);
    s.set("# edges", data().m_skin_surface.data()->triangulated_mixed_complex().number_of_finite_edges());
    stats.push_back(s);
    s.set("# faces", data().m_skin_surface.data()->triangulated_mixed_complex().number_of_finite_facets());
    stats.push_back(s);
    s.set("# cells", data().m_skin_surface.data()->triangulated_mixed_complex().number_of_finite_cells());
    stats.push_back(s);

    s.category = "skin surface mesh";
    s.set("# vertices", data().m_skin_surface_mesh.data().size_of_vertices());
    stats.push_back(s);
    s.set("# edges", data().m_skin_surface_mesh.data().size_of_halfedges());
    stats.push_back(s);
    s.set("# faces", data().m_skin_surface_mesh.data().size_of_facets());
    stats.push_back(s);
}
bool Model::clear()
{
    data().clear();
}

bool Model::load(const std::string &filename)
{
    data().clear();

    // Retrieve input balls:
    std::list<Weighted_point> input_points;
    extract_balls_from_pdb(
            filename.c_str(),
            input_points);

    if (input_points.empty())
    {
        Weighted_point wp;
        std::ifstream in(filename.c_str());
        while (in >> wp) input_points.push_back(wp);
    }

    std::cout << "#Balls: " << input_points.size() << std::endl;

    data().m_input_points.swap_data(input_points);

    return true;
}

bool Model::update() {
    bool result = true;

    result &= update_osg_input_points();
    result &= update_skin_surface();
    result &= update_skin_surface_mesh();
    result &= update_osg_skin_surface_mesh();

    return result;
}

bool Model::update_osg_input_points() {
    if (!(data().m_osg_input_points.is_up_to_date(data().m_input_points))) {
        osg::ref_ptr<osg::Geode> balls = data().m_osg_input_points.modify_data()->asGeode();

        assert(balls != NULL);
        if (balls->getNumDrawables() > 0)
            balls->removeDrawables(0, balls->getNumDrawables());

        BOOST_FOREACH(Weighted_point wp, data().m_input_points.data())
        {
            if (wp.weight() > 0) {
                osg::Vec3f pos(wp.x(), wp.y(), wp.z());

                osg::Drawable *drawable = OsgUtils::create_sphere(pos, sqrt(wp.weight()), 4);
                drawable->setUseDisplayList(false);
                drawable->setUseVertexBufferObjects(true);

                balls->addDrawable(drawable);
            }
        }

        // Update cache
        data().m_osg_input_points.make_up_to_date(data().m_input_points);
    }
}

void Model::show_balls(bool b)
{
    data().m_osg_input_points.modify_data()->setNodeMask(b?~0:0);
}
void Model::show_skin_surface(bool b)
{
    data().m_osg_skin_surface_mesh.modify_data()->setNodeMask(b?~0:0);
}

bool Model::update_skin_surface() {
    if (!(data().m_skin_surface.is_up_to_date(data().m_input_points) &&
          data().m_skin_surface.is_up_to_date(data().m_shrinkfactor))) {
        boost::shared_ptr<Skin_surface_3> skin(new Skin_surface_3(
                        data().m_input_points.data().begin(),
                        data().m_input_points.data().end(),
                        data().m_shrinkfactor.data(),
                        false));

        data().m_skin_surface.swap_data(skin);

        // Update cache
        data().m_skin_surface.make_up_to_date(data().m_input_points);
        data().m_skin_surface.make_up_to_date(data().m_shrinkfactor);
    }
    return true;
}

bool Model::update_skin_surface_mesh()
{
    if (!(data().m_skin_surface_mesh.is_up_to_date(data().m_skin_surface))) {
        Polyhedron mesh;
        boost::shared_ptr<Skin_surface_3> skin_surface = data().m_skin_surface.data();

        CGAL::mesh_skin_surface_3(*skin_surface, mesh);
        data().m_skin_surface_mesh.set_data(mesh);


        // Update cache
        data().m_skin_surface_mesh.make_up_to_date(data().m_skin_surface);
    }
    return true;
}
bool Model::update_osg_skin_surface_mesh()
{
    if (!(data().m_osg_skin_surface_mesh.is_up_to_date(data().m_skin_surface) &&
          data().m_osg_skin_surface_mesh.is_up_to_date(data().m_skin_surface_mesh))) {
        osg::ref_ptr<osg::Geode> geode = data().m_osg_skin_surface_mesh.modify_data();

        Polyhedron &p = data().m_skin_surface_mesh.data_non_const();
        const boost::shared_ptr<Skin_surface_3> &skin = data().m_skin_surface.data();

        if (geode->getNumDrawables() > 0)
            geode->removeDrawables(0, geode->getNumDrawables());

        std::vector<osg::Geometry *> geometries = CgalOsgUtils::convert_skin_mesh(*skin, p);
        int i=0;
        BOOST_FOREACH(osg::Geometry *geometry, geometries) {
            geometry->setUseDisplayList(false);
            geometry->setUseVertexBufferObjects(true);

            osg::Material *material = new osg::Material();
            osg::Vec4 color;
            switch (i++) {
            case 0: color = osg::Vec4(0,0,1,1); break;
            case 1: color = osg::Vec4(0,1,0,1); break;
            case 2: color = osg::Vec4(1,0,0,1); break;
            case 3: color = osg::Vec4(0,1,1,1); break;
            }
            material->setDiffuse(osg::Material::FRONT, color);
            geometry->getOrCreateStateSet()->setAttribute(material);

            geode->addDrawable(geometry);
        }

        // Update cache
        data().m_osg_skin_surface_mesh.make_up_to_date(data().m_skin_surface);
        data().m_osg_skin_surface_mesh.make_up_to_date(data().m_skin_surface_mesh);
    }
    return true;
}
void Model::color_skin_surface(bool b)
{
    osg::Material *mat = new osg::Material();
    mat->setDiffuse(osg::Material::FRONT, osg::Vec4(0.5, 0.5, 1, 1));

    osg::StateAttribute::Values v = osg::StateAttribute::OVERRIDE;
    if (b) v = osg::StateAttribute::OFF;

    data().m_osg_skin_surface_mesh.modify_data()->getOrCreateStateSet()->setAttribute(mat, v);
}

bool Model::subdivide_skin_surface_mesh()
{
    CGAL::subdivide_skin_surface_mesh_3(
                    *data().m_skin_surface.data(),
                    data().m_skin_surface_mesh.modify_data(),
                    1);
    return true;
}

void Model::set_shrink_factor(double shrink_factor)
{
    shrink_factor = std::max(0.01, std::min(1-0.01, shrink_factor));
    data().m_shrinkfactor.set_data(shrink_factor);
}
