#include <view/voronoi_view.h>

#include <boost/foreach.hpp>

#include <view/utils.h>

void VoronoiView::draw(QPainter &painter, const Regular &regular)
{
    QPen pen;
    pen.setColor(QColor(0, 255, 0));
    pen.setWidth(5);
    painter.setPen(pen);

    Segment s;
    Ray r;
    Line l;
    Regular::Finite_edges_iterator eit;
    for (eit = regular.finite_edges_begin(); eit != regular.finite_edges_end(); ++eit) {
        CGAL::Object o = regular.dual(eit);
        if (CGAL::assign(s, o))
            painter << s;
        else if (CGAL::assign(r, o))
            painter << r;
        else if (CGAL::assign(l,o))
            painter << l;

    }

}
