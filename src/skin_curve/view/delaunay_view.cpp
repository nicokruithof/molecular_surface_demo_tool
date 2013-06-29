#include <view/delaunay_view.h>

#include <boost/foreach.hpp>

#include <view/utils.h>

void DelaunayView::draw(QPainter &painter, const Regular &regular)
{
    QPen pen;
    pen.setColor(QColor(0, 0, 255));
    pen.setWidth(5);
    painter.setPen(pen);

    Regular::Finite_edges_iterator eit;
    for (eit = regular.finite_edges_begin(); eit != regular.finite_edges_end(); ++eit) {
        painter << regular.segment(eit);
    }

}
