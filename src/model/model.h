#ifndef MODEL_MODEL_H_
#define MODEL_MODEL_H_

#include <string>

#include <boost/shared_ptr.hpp>

#include <osg/Group>

class ModelData;

class Model {
public:
    Model();
    virtual ~Model();

    // Access functions
    osg::ref_ptr<osg::Node> scene() const;

    // Actions triggered by the UI
    bool load(const std::string &filename);
    bool update();

    void show_balls(bool b);

private:
    bool update_osg_input_points();
    bool update_regular_triangulation();
    bool update_skin_surface();
private:
    boost::shared_ptr<ModelData> m_model_data;
};

#endif /* MODEL_MODEL_H_ */
