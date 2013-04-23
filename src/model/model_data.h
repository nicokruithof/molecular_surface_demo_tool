#ifndef MODEL_MODEL_DATA_H_
#define MODEL_MODEL_DATA_H_

#include <vector>

#include <model/types.h>

#include <osg/Group>

#include <model/cached_class.h>

class ModelData {
public:
    ModelData()
    : m_input_points("input_points")
    , m_osg_input_points("osg_input_points", new osg::Group())
    , m_shrinkfactor("shrink_factor", 0.5)
    , m_regular_triangulation("regular_triangulation")
    {
        clear();
    }
    ~ModelData() {}

    void clear() {
        m_scene = new osg::Group();
        m_input_points.clear();
        m_shrinkfactor.set_data(0.5);
        m_molecular_systems.clear();
        m_regular_triangulation.clear();
    }

public: // Generic
    // Root node for the entire visual scene
    osg::ref_ptr<osg::Group> m_scene;

public: // Atoms
    CachedClass< std::list<Weighted_point> > m_input_points;
    //Container for molecular system
    std::vector<System>       m_molecular_systems;

    CachedClass< osg::ref_ptr<osg::Group> > m_osg_input_points;

public: // Skin surface
    CachedClass< double >                  m_shrinkfactor;
    CachedClass< Regular_triangulation_3 > m_regular_triangulation;
};

#endif /* MODEL_MODEL_DATA_H_ */
