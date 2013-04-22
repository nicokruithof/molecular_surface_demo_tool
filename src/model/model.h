#ifndef MODEL_MODEL_H_
#define MODEL_MODEL_H_

#include <string>

#include <boost/shared_ptr.hpp>

class ModelData;

class Model {
public:
    Model();
    virtual ~Model();

    bool load(const std::string &filename);

private:
    boost::shared_ptr<ModelData> m_model_data;
};

#endif /* MODEL_MODEL_H_ */
