#include <view/skin_curve_view.h>
#include <view/utils.h>

#include <boost/foreach.hpp>

void SkinCurveView::draw(QPainter &painter, Regular &regular, double shrink_factor)
{
    shrink_factor = std::min(std::max(shrink_factor, 0.001), 0.999);

    QPen pen;
    pen.setColor(QColor(255, 0, 0));
    //pen.setWidth(5);
    painter.setPen(pen);

    Regular::Finite_vertices_iterator vit;
    for (vit = regular.finite_vertices_begin(); vit != regular.finite_vertices_end(); ++vit) {
        draw(painter, regular, vit, shrink_factor);
    }
    Regular::Finite_faces_iterator fit;
    for (fit = regular.finite_faces_begin(); fit != regular.finite_faces_end(); ++fit) {
        draw(painter, regular, fit, shrink_factor);
    }

}

void SkinCurveView::draw(QPainter &painter, Regular &regular, Regular::Finite_vertices_iterator &vit, double shrink_factor)
{
    std::list<Segment> segments;
    Weighted_point wp(vit->point(), shrink_factor*vit->point().weight());

    discretize_segments(wp, segments);

    if (regular.dimension() > 0) {
        Regular::Geom_traits gt = regular.geom_traits();
        Regular::Vertex_circulator adj_vit = regular.incident_vertices(vit);
        Regular::Vertex_circulator adj_vit_start = adj_vit;
        do {
            if (!regular.is_infinite(adj_vit)) {
                Line l = gt.construct_radical_axis_2_object()(wp, adj_vit->point());
                l = Line(wp + shrink_factor*(l.point(0)-wp), l.direction());
                clip(segments, l);
                //clip(segments, Line(l.point(0), l.point(1)));
            }
        } while (++adj_vit != adj_vit_start);
    }

    painter << segments;
}

void SkinCurveView::draw(QPainter &painter, Regular &/*regular*/, Regular::Finite_faces_iterator &fit, double shrink_factor)
{
    Weighted_point wp = focus(fit);
    wp = Weighted_point(wp, (1-shrink_factor) * wp.weight());

    std::list<Segment> segments;
    discretize_segments(wp, segments);

    for (int i=0; i<3; ++i) {
        Line l(wp + (1-shrink_factor)*(fit->vertex(i)->point()-wp),
               wp + (1-shrink_factor)*(fit->vertex((i+1)%3)->point()-wp));
        clip(segments, l);
        painter << l;
//        Regular::Geom_traits gt = regular.geom_traits();
//        Regular::Vertex_circulator adj_vit = regular.incident_vertices(vit);
//        Regular::Vertex_circulator adj_vit_start = adj_vit;
//        do {
//            if (!regular.is_infinite(adj_vit)) {
//                Line l = gt.construct_radical_axis_2_object()(wp, adj_vit->point());
//                clip(segments, Line(l.point(0), l.point(1)));
//            }
//        } while (++adj_vit != adj_vit_start);
//    }

    }

    painter << segments;
}
