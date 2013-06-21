#ifndef VIEW_UNION_OF_BALLS_VIEW_H
#define VIEW_UNION_OF_BALLS_VIEW_H

#include <qpainter.h>

#include <model/model.h>

class UnionOfBallsView {
public:
    virtual void draw(QPainter &painter, Regular &regular);

private:
    void draw(QPainter &painter, Regular &regular, Regular::Finite_vertices_iterator &vit);

protected:
    void discretize_segments(const Weighted_point &wp, std::list<Segment> &segments);

    void clip(std::list<Segment> &segments, const Line &line);
};

#endif // VIEW_UNION_OF_BALLS_VIEW_H
