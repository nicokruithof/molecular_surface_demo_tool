#include <view/mixed_complex_view.h>

#include <boost/foreach.hpp>

#include <view/utils.h>

void MixedComplexView::draw(QPainter &painter, const Regular &regular, double s)
{
    QPen pen;
    pen.setColor(QColor(0, 0, 0));
    //pen.setWidth(5);
    painter.setPen(pen);

    Segment seg;
    Ray ray;
    Line line;
    Regular::Finite_edges_iterator eit;
    for (eit = regular.finite_edges_begin(); eit != regular.finite_edges_end(); ++eit) {
        const Weighted_point &p0 = eit->first->vertex((eit->second+1)%3)->point();
        const Weighted_point &p1 = eit->first->vertex((eit->second+2)%3)->point();

        CGAL::Object o = regular.dual(eit);
        if (CGAL::assign(seg, o)) {
            painter << Segment(p0 + s * (seg[0]-p0), p0 + s * (seg[1]-p0));
            painter << Segment(p1 + s * (seg[0]-p1), p1 + s * (seg[1]-p1));
            painter << Segment(p0 + s * (seg[0]-p0), p1 + s * (seg[0]-p1));
            painter << Segment(p0 + s * (seg[1]-p0), p1 + s * (seg[1]-p1));
        }
        else if (CGAL::assign(ray, o)) {
            painter << Ray(p0 + s * (ray.source()-p0), ray.direction());
            painter << Ray(p1 + s * (ray.source()-p1), ray.direction());
            painter << Segment(p0 + s * (ray.source()-p0), p1 + s * (ray.source()-p1));
        }
        else if (CGAL::assign(line,o)) {
            painter << Line(p0 + s * (line.point(0)-p0), line.direction());
            painter << Line(p1 + s * (line.point(0)-p1), line.direction());
        }
    }

}
