#ifndef MODEL_MODEL_H
#define MODEL_MODEL_H

#include <model/types.h>

class Model {
public:
  Model();

  const Regular &regular();

  void set_shrink_factor(double shrink_factor);
  double shrink_factor() const { return m_shrink_factor; }

  void set_probe_radius(double probe_radius);
  void set_multiply_with_shrink_factor(bool b);

  void clear();
  void insert(const Weighted_point &wp);
  void remove(const Weighted_point &wp);

  const std::vector<Weighted_point> &points();
private:
  std::vector<Weighted_point> m_points;

  bool m_regular_is_valid;
  Regular m_regular;

  double m_probe_radius;

  double m_shrink_factor;
  bool m_multiply_with_shrink_factor;
};

#endif // MODEL_MODEL_H
