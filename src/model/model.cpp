#include "model/model.h"
#include "model/model_data.h"

#include <boost/foreach.hpp>

#include <osg/Geode>
#include <osg/ShapeDrawable>

#include "extract_balls_from_pdb.h"

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
    m_model_data->m_input_points.set_data(input_points);

    std::cout << m_model_data->m_input_points.data().size() << std::endl;
    return true;
}

bool Model::update() {
    bool result = true;
    result &= update_osg_input_points();
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
                osg::Sphere *sphere = new osg::Sphere(pos, sqrt(wp.weight()));
                osg::ShapeDrawable *drawable = new osg::ShapeDrawable(sphere);
                result->addDrawable(drawable);
            }
        }

        m_model_data->m_scene->removeChild(m_model_data->m_osg_input_points.data());
        m_model_data->m_osg_input_points.set_data(osg_input_points);
        m_model_data->m_scene->addChild(m_model_data->m_osg_input_points.data());

        // Update cache
        m_model_data->m_osg_input_points.make_up_to_date(m_model_data->m_input_points);
    }
}
