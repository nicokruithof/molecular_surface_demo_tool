#include <view/connolly_view.h>
#include <view/utils.h>

#include <boost/foreach.hpp>

void ConnollyView::draw(QPainter &painter, const Regular &regular, double probe_radius)
{
    QPen pen;
    pen.setColor(QColor(0, 0, 255));
    pen.setWidth(3);
    painter.setPen(pen);

//    Regular::Finite_vertices_iterator vit;
//    for (vit = regular.finite_vertices_begin(); vit != regular.finite_vertices_end(); ++vit) {
//        draw(painter, regular, probe_radius, vit);
//    }

    Regular::Finite_edges_iterator eit;
    for (eit = regular.finite_edges_begin(); eit != regular.finite_edges_end(); ++eit) {
        draw(painter, regular, probe_radius, eit);
    }
}

void ConnollyView::draw(QPainter &painter, const Regular &regular, double probe_radius, Regular::Finite_vertices_iterator &vit)
{
    std::list<Segment> segments;
    generate_circle(vit->point(), segments, 2);

    if (regular.dimension() > 0) {
        Weighted_point wp = vit->point();
        const Regular::Geom_traits &gt = regular.geom_traits();
        Regular::Vertex_circulator adj_vit = regular.incident_vertices(vit);
        Regular::Vertex_circulator adj_vit_start = adj_vit;
        do {
            if (!regular.is_infinite(adj_vit)) {
                Line l = gt.construct_radical_axis_2_object()(wp, adj_vit->point());
                clip(segments, l);
            }
        } while (++adj_vit != adj_vit_start);
    }

    double probe_weight = probe_radius*probe_radius;
    BOOST_FOREACH(const Segment &s, segments) {
        painter << Weighted_point(s.point(0), probe_weight);
    }
}

void ConnollyView::draw(QPainter &painter, const Regular &regular, double probe_radius, Regular::Finite_edges_iterator &eit)
{
    const double probe_weight = probe_radius*probe_radius;

    if (regular.dimension() > 0) {
        Segment s = regular.segment(eit);
        Weighted_point f = focus(eit);
        if (f.weight() < 0) {
            Vector orth_v = regular.segment(eit).direction().perpendicular(CGAL::COUNTERCLOCKWISE).vector();
            orth_v = orth_v / sqrt(orth_v.squared_length());

            double len = sqrt(-f.weight());
            Bare_point p0 = f+len*orth_v;
            Bare_point p1 = f-len*orth_v;
            bool p0_ok = true, p1_ok = true;
            if (regular.dimension()==2) {
                Regular::Vertex_handle vh = eit->first->vertex(eit->second);
                if (!regular.is_infinite(vh)) {
                    Weighted_point wp3 = vh->point();
                    p0_ok &= (wp3-p0).squared_length() > wp3.weight();
                    p1_ok &= (wp3-p1).squared_length() > wp3.weight();
                }
                Regular::Face_handle nb = eit->first->neighbor(eit->second);
                vh = nb->vertex(nb->index(eit->first));
                if (!regular.is_infinite(vh)) {
                    Weighted_point wp3 = vh->point();
                    p0_ok &= (wp3-p0).squared_length() > wp3.weight();
                    p1_ok &= (wp3-p1).squared_length() > wp3.weight();
                }
            }
            if (p0_ok) {
                std::list<Segment> segments;
                Vector v0 = (s.point(0)-p0);
                Vector v1 = (s.point(1)-p0);
                segments.push_back(Segment(
                                p0 + probe_radius/sqrt(v0.squared_length()) * v0,
                                p0 + probe_radius/sqrt(v1.squared_length()) * v1));
                subdiv_circle(Weighted_point(p0, probe_weight), segments, 4);
                if (p1_ok)
                    clip(segments, s.supporting_line());
                painter << segments;
            }
            if (p1_ok) {
                std::list<Segment> segments;
                Vector v0 = (s.point(0)-p1);
                Vector v1 = (s.point(1)-p1);
                segments.push_back(Segment(
                                p1 + probe_radius/sqrt(v0.squared_length()) * v0,
                                p1 + probe_radius/sqrt(v1.squared_length()) * v1));
                subdiv_circle(Weighted_point(p1, probe_weight), segments, 4);
                if (p0_ok)
                    clip(segments, s.supporting_line().opposite());
                painter << segments;
            }
        }
    }
}
