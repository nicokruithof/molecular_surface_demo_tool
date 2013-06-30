#ifndef VIEW_UNION_OF_BALLS_VIEW_H
#define VIEW_UNION_OF_BALLS_VIEW_H

#include <qpainter.h>

#include <model/model.h>

class UnionOfBallsView {
public:
    virtual void draw(QPainter &painter, const Regular &regular);

private:
    void draw(QPainter &painter, const Regular &regular, Regular::Finite_vertices_iterator &vit);

protected:
    void generate_circle(const Weighted_point &wp, std::list<Segment> &segments, int subdiv=8);
    void subdiv_circle(const Weighted_point &wp, std::list<Segment> &segments, int subdiv=8);

    void clip(std::list<Segment> &segments, const Line &line);

    inline Weighted_point focus(Regular::Finite_edges_iterator eit) {
        Regular::Face_handle fh = eit->first;
        int i = eit->second;
        return focus(fh->vertex((i+1)%3)->point(), fh->vertex((i+2)%3)->point());
    }
    inline Weighted_point focus(Regular::Finite_faces_iterator fit) {
        return focus(fit->vertex(0)->point(), fit->vertex(1)->point(), fit->vertex(2)->point());
    }
    Weighted_point focus(const Weighted_point &wp1, const Weighted_point &wp2) const;
    Weighted_point focus(const Weighted_point &wp1, const Weighted_point &wp2, const Weighted_point &wp3) const;
};

#endif // VIEW_UNION_OF_BALLS_VIEW_H
