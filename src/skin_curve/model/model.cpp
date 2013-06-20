#include <model/model.h>

Model::Model()
: m_shrink_factor(0.5)
{
}

void Model::insert(const Weighted_point &wp)
{
    m_regular.insert(wp);
}

void Model::get_points(std::vector<Weighted_point> &pts)
{
    Regular::Finite_vertices_iterator vit;
    for (vit = m_regular.finite_vertices_begin(); vit != m_regular.finite_vertices_end(); ++vit) {
        pts.push_back(vit->point());
    }
}
