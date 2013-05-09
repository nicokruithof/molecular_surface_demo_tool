#ifndef MODEL_MODEL_DATA_H_
#define MODEL_MODEL_DATA_H_

#include <vector>

#include <model/types.h>

#include <osg/Group>
#include <osg/Geode>

#include <model/cached_class.h>

class ModelData {
public:
    ModelData()
    : m_scene(new osg::Group())
    , m_input_points("input_points")
    , m_osg_input_points("osg_input_points", new osg::Geode())
    , m_shrinkfactor("shrink_factor", 0.5)
    , m_skin_surface("skin_surface")
    , m_skin_surface_mesh("skin_surface_mesh")
    , m_osg_skin_surface_mesh("osg_skin_surface_mesh", new osg::Geode())
    {
        m_scene->addChild(m_osg_input_points.data());
        m_scene->addChild(m_osg_skin_surface_mesh.data());

        clear();
    }
    ~ModelData() {}

    void clear() {
        m_input_points.clear();
        m_shrinkfactor.set_data(0.5);
        m_skin_surface.set_data(boost::shared_ptr<Skin_surface_3>());

        osg::Geode *geode;
        geode = m_osg_input_points.modify_data();
        if (geode->getNumDrawables() > 0)
            geode->removeDrawables(0, geode->getNumDrawables());
        geode = m_osg_skin_surface_mesh.modify_data();
        if (geode->getNumDrawables() > 0)
            geode->removeDrawables(0, geode->getNumDrawables());
    }

public: // Generic
    // Root node for the entire visual scene
    osg::ref_ptr<osg::Group> m_scene;

public: // Atoms
    CachedClass< std::list<Weighted_point> > m_input_points;

    CachedClass< osg::ref_ptr<osg::Geode> > m_osg_input_points;

public: // Skin surface
    CachedClass< double >                  m_shrinkfactor;

    CachedClass< boost::shared_ptr<Skin_surface_3> > m_skin_surface;
    CachedClass< Polyhedron >                        m_skin_surface_mesh;
    CachedClass< osg::ref_ptr<osg::Geode> >          m_osg_skin_surface_mesh;
};

#endif /* MODEL_MODEL_DATA_H_ */
