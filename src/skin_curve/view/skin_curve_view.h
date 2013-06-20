#ifndef VIEW_SKIN_CURVE_VIEW_H
#define VIEW_SKIN_CURVE_VIEW_H

#include <qpainter.h>

#include <model/model.h>

class SkinCurveView {
public:
    void draw(QPainter &painter, Regular &regular, double shrink_factor);

private:
    void draw(QPainter &painter, Regular::Finite_vertices_iterator &vit, double shrink_factor);

private:
    void discretize_segments(const Weighted_point &wp, std::list<Segment> &segments);
};

#endif // VIEW_SKIN_CURVE_VIEW_H
