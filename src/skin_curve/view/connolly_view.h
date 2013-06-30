#ifndef VIEW_CONNOLLY_VIEW_H
#define VIEW_CONNOLLY_VIEW_H

#include <view/union_of_balls_view.h>

#include <qpainter.h>

#include <model/model.h>

class ConnollyView : public UnionOfBallsView {
public:
    virtual void draw(QPainter &painter, const Regular &regular, double probe_radius);

private:
    void draw(QPainter &painter, const Regular &regular, double probe_radius, Regular::Finite_vertices_iterator &vit);
    void draw(QPainter &painter, const Regular &regular, double probe_radius, Regular::Finite_edges_iterator &eit);
};

#endif // VIEW_CONNOLLY_VIEW_H
