#ifndef VIEW_SKIN_CURVE_VIEW_H
#define VIEW_SKIN_CURVE_VIEW_H

#include <qpainter.h>

#include <view/union_of_balls_view.h>

#include <model/model.h>

class SkinCurveView : public UnionOfBallsView {
public:
    void draw(QPainter &painter, Regular &regular, double shrink_factor);

private:
    void draw(QPainter &painter, Regular &regular, Regular::Finite_vertices_iterator &vit, double shrink_factor);
    void draw(QPainter &painter, Regular &regular, Regular::Finite_faces_iterator &vit, double shrink_factor);
};

#endif // VIEW_SKIN_CURVE_VIEW_H
