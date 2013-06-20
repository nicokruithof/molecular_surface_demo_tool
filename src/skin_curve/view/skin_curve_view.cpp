#include <view/skin_curve_view.h>

#include <boost/foreach.hpp>

void SkinCurveView::draw(QPainter &painter, Regular &regular, double shrink_factor)
{
    QPen pen;
    pen.setColor(QColor(255, 0, 0));
    //pen.setWidth(5);
    painter.setPen(pen);

    Regular::Finite_vertices_iterator vit;
    for (vit = regular.finite_vertices_begin(); vit != regular.finite_vertices_end(); ++vit) {
        draw(painter, vit, shrink_factor);
    }

}

void SkinCurveView::draw(QPainter &painter, Regular::Finite_vertices_iterator &vit, double shrink_factor)
{
    std::list<Segment> segments;
    discretize_segments(Weighted_point(vit->point(), shrink_factor*vit->point().weight()), segments);

    BOOST_FOREACH(Segment s, segments) {
        painter.drawLine(s.point(0).x(), s.point(0).y(), s.point(1).x(), s.point(1).y());
    }
}

void SkinCurveView::discretize_segments(const Weighted_point &wp, std::list<Segment> &segments)
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
