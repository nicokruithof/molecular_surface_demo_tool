#include "model/model.h"
#include "model/model_data.h"

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

}

