#ifndef CONTROLLER_CONTROLLER_H
#define CONTROLLER_CONTROLLER_H

#include <boost/shared_ptr.hpp>

#include <model/model.h>

class Controller {
public:
    Controller();
    void set_model(Model *model);

    void insert(double x, double y, double w);

private:
    Model *m_model;
};

#endif  // CONTROLLER_CONTROLLER_H
