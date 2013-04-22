#include "model/model.h"
#include "model/model_data.h"

#include "extract_balls_from_pdb.h"

Model::Model()
: m_model_data(new ModelData())
{
}

Model::~Model() {
}

bool Model::load(const std::string &filename)
{
    m_model_data->clear();

    // Retrieve input balls:
    extract_balls_from_pdb<K>(
            filename.c_str(),
            m_model_data->m_molecular_systems,
            std::back_inserter(m_model_data->m_input_points));

    return true;
}
