#include "model/model.h"
#include "model/model_data.h"

#include <fstream>

#include <boost/foreach.hpp>

#include <osg/Geode>
#include <osg/CullFace>

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

bool Model::load(const std::string &filename)
{
    data().clear();

    // Retrieve input balls:
    std::list<Weighted_point> input_points;
    extract_balls_from_pdb<K>(
            filename.c_str(),
            data().m_molecular_systems,
            std::back_inserter(input_points));

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
    result &= update_regular_triangulation();
    result &= update_skin_surface();
    result &= update_skin_surface_mesh();
    result &= update_osg_skin_surface_mesh();

    return result;
}

bool Model::update_osg_input_points() {
    if (!(data().m_osg_input_points.is_up_to_date(data().m_input_points))) {
        osg::ref_ptr<osg::Group> osg_input_points = new osg::Group();
        osg::ref_ptr<osg::Geode> result = new osg::Geode();
        osg_input_points->addChild(result);

        BOOST_FOREACH(Weighted_point wp, data().m_input_points.data())
        {
            if (wp.weight() > 0) {
                osg::Vec3f pos(wp.x(), wp.y(), wp.z());
                result->addDrawable(OsgUtils::create_sphere(pos, sqrt(wp.weight())));
            }
        }
        osg::StateSet* state = result->getOrCreateStateSet();
        state->setAttributeAndModes(new osg::CullFace());

        data().m_scene->removeChild(data().m_osg_input_points.data());
        data().m_osg_input_points.set_data(osg_input_points);
        data().m_scene->addChild(data().m_osg_input_points.data());

        // Update cache
        data().m_osg_input_points.make_up_to_date(data().m_input_points);
    }
}

void Model::show_balls(bool b)
{
    data().m_osg_input_points.modify_data()->setNodeMask(b?~0:0);
}

bool Model::update_regular_triangulation() {
    if (!(data().m_regular_triangulation.is_up_to_date(data().m_input_points))) {
        Regular_triangulation_3 triang(
                        data().m_input_points.data().begin(),
                        data().m_input_points.data().end());

        data().m_regular_triangulation.swap_data(triang);

        // Update cache
        data().m_regular_triangulation.make_up_to_date(data().m_input_points);
    }
    return true;
}

bool Model::update_skin_surface() {
    if (!(data().m_skin_surface.is_up_to_date(data().m_input_points) ||
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
    if (!(data().m_osg_skin_surface_mesh.is_up_to_date(data().m_skin_surface) ||
          data().m_osg_skin_surface_mesh.is_up_to_date(data().m_skin_surface_mesh))) {
        osg::ref_ptr<osg::Geode> node = new osg::Geode();
        Polyhedron &p = data().m_skin_surface_mesh.data_non_const();
        const boost::shared_ptr<Skin_surface_3> &skin = data().m_skin_surface.data();

        osg::Geometry *geometry = CgalOsgUtils::convert_mesh_with_normals(*skin, p);
        node->addDrawable(geometry);

        data().m_scene->removeChild(data().m_osg_skin_surface_mesh.data());
        data().m_osg_skin_surface_mesh.set_data(node);
        data().m_scene->addChild(data().m_osg_skin_surface_mesh.data());

        // Update cache
        data().m_osg_skin_surface_mesh.make_up_to_date(data().m_skin_surface);
        data().m_osg_skin_surface_mesh.make_up_to_date(data().m_skin_surface_mesh);
    }
    return true;
}
