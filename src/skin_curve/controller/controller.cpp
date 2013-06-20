#include <controller/controller.h>

#include <iostream>

Controller::Controller()
: m_model(NULL)
{
}

void Controller::set_model(Model* model)
{
    m_model = model;
}

void Controller::insert(double x, double y, double w)
{
    if (m_model != NULL)
        m_model->insert(Weighted_point(Bare_point(x,y), w));
}
