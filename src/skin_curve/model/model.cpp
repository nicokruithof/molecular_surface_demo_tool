#include <model/model.h>

#include <boost/foreach.hpp>

Model::Model()
: m_regular_is_valid(true)
, m_shrink_factor(0.5)
{
}

void Model::clear()
{
    m_points.clear();
    m_regular_is_valid = false;
}
void Model::insert(const Weighted_point &wp)
{
    m_points.push_back(wp);
    m_regular_is_valid = false;
}
void Model::remove(const Weighted_point &wp)
{
    // really remove all elements with value wp
    m_points.erase(std::remove(m_points.begin(), m_points.end(), wp), m_points.end());
    m_regular_is_valid = false;
}

const Regular &Model::regular()
{
    if (!m_regular_is_valid) {
        m_regular.clear();
        BOOST_FOREACH(Weighted_point wp, m_points) {
            double weight = wp.weight();
            if (weight >= 0) {
                double radius = sqrt(weight) + m_probe_radius;
                weight = radius * radius;
                if (m_multiply_with_shrink_factor)
                    weight /= m_shrink_factor;

                m_regular.insert(Weighted_point(wp, weight));
            }
        }
    }
    return m_regular;
}


const std::vector<Weighted_point> &Model::points()
{
    return m_points;
}

void Model::set_shrink_factor(double shrink_factor) {
    m_shrink_factor = std::min(std::max(shrink_factor, 0.01), 0.99);

    m_regular_is_valid &= !m_multiply_with_shrink_factor;
}

void Model::set_probe_radius(double probe_radius) {
    m_probe_radius = probe_radius;
    m_regular_is_valid = false;
}
void Model::set_multiply_with_shrink_factor(bool b) {
    m_multiply_with_shrink_factor = b;
    m_regular_is_valid = false;
}
