#ifndef MODEL_MODEL_DATA_H_
#define MODEL_MODEL_DATA_H_

#include <model/types.h>
#include <vector>

class ModelData {
public:
    ModelData() { clear(); }
    ~ModelData() {}

    void clear() {
        m_input_points.clear();
        m_shrinkfactor = 0.5;
        m_molecular_systems.clear();
        m_regular_triangulation.clear();
    }
//private:

    std::list<Weighted_point> m_input_points;
    double                    m_shrinkfactor;
    //Container for molecular system
    std::vector<System>       m_molecular_systems;

    Regular_triangulation_3 m_regular_triangulation;
};

#endif /* MODEL_MODEL_DATA_H_ */
