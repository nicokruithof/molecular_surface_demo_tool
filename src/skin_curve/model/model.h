#ifndef MODEL_MODEL_H
#define MODEL_MODEL_H

#include <model/types.h>

class Model {
public:
  Model();

  Regular &regular() { return m_regular; }

  void set_shrink_factor(double shrink_factor) { m_shrink_factor = shrink_factor; }
  double shrink_factor() { return m_shrink_factor; }

  void insert(const Weighted_point &wp);
  void get_points(std::vector<Weighted_point> &pts);
private:
  Regular m_regular;

  double m_shrink_factor;
};

#endif // MODEL_MODEL_H
