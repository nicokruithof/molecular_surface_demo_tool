#include "model/model.h"
#include "model/model_data.h"

#include <fstream>

#include <boost/foreach.hpp>

#include <osg/Geode>
#include <osg/CullFace>

#include <model/extract_balls_from_pdb.h>

#include <osg/osg_utils.h>

Model::Model()
: m_model_data(new ModelData())
{
}

Model::~Model() {
}

osg::ref_ptr<osg::Node> Model::scene() const {
    return m_model_data->m_scene;
}

bool Model::load(const std::string &filename)
{
    m_model_data->clear();

    // Retrieve input balls:
    std::list<Weighted_point> input_points;
    extract_balls_from_pdb<K>(
            filename.c_str(),
            m_model_data->m_molecular_systems,
            std::back_inserter(input_points));

    if (input_points.empty())
    {
        Weighted_point wp;
        std::ifstream in(filename.c_str());
        while (in >> wp) input_points.push_back(wp);
    }

    std::cout << "#Balls: " << input_points.size() << std::endl;

    m_model_data->m_input_points.swap_data(input_points);

    return true;
}

bool Model::update() {
    bool result = true;

    result &= update_osg_input_points();
    //result &= update_regular_triangulation();
    result &= update_skin_surface();

    return result;
}

bool Model::update_osg_input_points() {
    if (!(m_model_data->m_osg_input_points.is_up_to_date(m_model_data->m_input_points))) {
        osg::ref_ptr<osg::Group> osg_input_points = new osg::Group();
        osg::ref_ptr<osg::Geode> result = new osg::Geode();
        osg_input_points->addChild(result);

        BOOST_FOREACH(Weighted_point wp, m_model_data->m_input_points.data())
        {
            if (wp.weight() > 0) {
                osg::Vec3f pos(wp.x(), wp.y(), wp.z());
                result->addDrawable(OsgUtils::create_sphere(pos, sqrt(wp.weight())));
            }
        }
        osg::StateSet* state = result->getOrCreateStateSet();
        state->setAttributeAndModes(new osg::CullFace());

        m_model_data->m_scene->removeChild(m_model_data->m_osg_input_points.data());
        m_model_data->m_osg_input_points.set_data(osg_input_points);
        m_model_data->m_scene->addChild(m_model_data->m_osg_input_points.data());

        // Update cache
        m_model_data->m_osg_input_points.make_up_to_date(m_model_data->m_input_points);
    }
}

void Model::show_balls(bool b)
{
    m_model_data->m_osg_input_points.modify_data()->setNodeMask(b?~0:0);
}

bool Model::update_regular_triangulation() {
    if (!(m_model_data->m_regular_triangulation.is_up_to_date(m_model_data->m_input_points))) {
        Regular_triangulation_3 triang(
                        m_model_data->m_input_points.data().begin(),
                        m_model_data->m_input_points.data().end());

        m_model_data->m_regular_triangulation.swap_data(triang);

        // Update cache
        m_model_data->m_regular_triangulation.make_up_to_date(m_model_data->m_input_points);
    }
}

bool Model::update_skin_surface() {
    if (!(m_model_data->m_skin_surface.is_up_to_date(m_model_data->m_input_points) ||
          m_model_data->m_skin_surface.is_up_to_date(m_model_data->m_shrinkfactor))) {
        boost::shared_ptr<Skin_surface_3> skin(new Skin_surface_3(
                        m_model_data->m_input_points.data().begin(),
                        m_model_data->m_input_points.data().end(),
                        m_model_data->m_shrinkfactor.data(),
                        false));

        m_model_data->m_skin_surface.swap_data(skin);

        // Update cache
        m_model_data->m_skin_surface.make_up_to_date(m_model_data->m_input_points);
        m_model_data->m_skin_surface.make_up_to_date(m_model_data->m_shrinkfactor);
    }
}

