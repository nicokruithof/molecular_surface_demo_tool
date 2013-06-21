#include <view/union_of_balls_view.h>
#include <view/utils.h>

#include <boost/foreach.hpp>

void UnionOfBallsView::draw(QPainter &painter, Regular &regular)
{
    QPen pen;
    pen.setColor(QColor(255, 0, 0));
    //pen.setWidth(5);
    painter.setPen(pen);

    Regular::Finite_vertices_iterator vit;
    for (vit = regular.finite_vertices_begin(); vit != regular.finite_vertices_end(); ++vit) {
        draw(painter, regular, vit);
    }

}

void UnionOfBallsView::draw(QPainter &painter, Regular &regular, Regular::Finite_vertices_iterator &vit)
{
    std::list<Segment> segments;
    discretize_segments(vit->point(), segments);

    if (regular.dimension() > 0) {
        Weighted_point wp = vit->point();
        Regular::Geom_traits gt = regular.geom_traits();
        Regular::Vertex_circulator adj_vit = regular.incident_vertices(vit);
        Regular::Vertex_circulator adj_vit_start = adj_vit;
        do {
            if (!regular.is_infinite(adj_vit)) {
                Line l = gt.construct_radical_axis_2_object()(wp, adj_vit->point());
                clip(segments, l);
            }
        } while (++adj_vit != adj_vit_start);
    }

    BOOST_FOREACH(Segment s, segments) {
        painter.drawLine(s.point(0).x(), s.point(0).y(), s.point(1).x(), s.point(1).y());
    }
}

void UnionOfBallsView::discretize_segments(const Weighted_point &wp, std::list<Segment> &segments)
{
    segments.clear();
    if (wp.weight() <= 0) return;

    double r = sqrt(wp.weight());
    Vector dx(r, 0), dy(0, r);
    segments.push_back(Segment(wp+dx, wp+dy));
    segments.push_back(Segment(wp+dy, wp-dx));
    segments.push_back(Segment(wp-dx, wp-dy));
    segments.push_back(Segment(wp-dy, wp+dx));

    std::list<Segment> segments2;
    for (int i=0; i<5; ++i) {
        segments2.clear();
        BOOST_FOREACH(Segment &s, segments) {
            Bare_point m = CGAL::midpoint(s[0], s[1]);
            Vector v = m-wp;
            m = wp + sqrt(wp.weight()/v.squared_length()) * v;
            segments2.push_back(Segment(s[0], m));
            segments2.push_back(Segment(m, s[1]));
        }
        segments.swap(segments2);
    }


}

void UnionOfBallsView::clip(std::list<Segment> &segments, const Line &line)
{
    std::list<Segment>::iterator it = segments.begin();
    while (it != segments.end()) {
        CGAL::Oriented_side s0 = line.oriented_side((*it)[0]);
        CGAL::Oriented_side s1 = line.oriented_side((*it)[1]);
        if (s0 == CGAL::RIGHT_TURN || s1 == CGAL::RIGHT_TURN) {
            std::list<Segment>::iterator it2 = it;
            ++it;
            segments.erase(it2);
        } else {
            ++it;
        }
    }
}
