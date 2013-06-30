#include <view/skin_curve_view.h>
#include <view/utils.h>

#include <boost/foreach.hpp>

void SkinCurveView::draw(QPainter &painter, const Regular &regular, double shrink_factor, bool use_colors)
{
    stepSize = 1;

    QPen pen;
    pen.setColor(QColor(255, 0, 0));
    pen.setWidth(1);
    painter.setPen(pen);

    if (use_colors) pen.setColor(QColor(255, 0, 0));
    painter.setPen(pen);
    Regular::Finite_vertices_iterator vit;
    for (vit = regular.finite_vertices_begin(); vit != regular.finite_vertices_end(); ++vit) {
        draw(painter, regular, vit, shrink_factor);
    }
    if (use_colors) pen.setColor(QColor(0, 255, 0));
    painter.setPen(pen);
    Regular::Finite_edges_iterator eit;
    for (eit = regular.finite_edges_begin(); eit != regular.finite_edges_end(); ++eit) {
        draw(painter, regular, eit, shrink_factor);
    }
    if (use_colors) pen.setColor(QColor(0, 0, 255));
    painter.setPen(pen);
    Regular::Finite_faces_iterator fit;
    for (fit = regular.finite_faces_begin(); fit != regular.finite_faces_end(); ++fit) {
        draw(painter, regular, fit, shrink_factor);
    }

}

void SkinCurveView::draw(QPainter &painter, const Regular &regular, Regular::Finite_vertices_iterator &vit, double shrink_factor)
{
    std::list<Segment> segments;
    Weighted_point wp = vit->point();

    generate_circle(Weighted_point(wp, shrink_factor*wp.weight()), segments);

    if (regular.dimension() > 0) {
        Regular::Vertex_circulator adj_vit = regular.incident_vertices(vit);
        Regular::Vertex_circulator adj_vit_start = adj_vit;
        do {
            if (!regular.is_infinite(adj_vit)) {
                Direction d = (wp-adj_vit->point()).perpendicular(CGAL::CLOCKWISE).direction();
                Bare_point p = focus(wp, adj_vit->point());
                p = wp + shrink_factor * (p-wp);
                clip(segments, Line(p, d));
            }
        } while (++adj_vit != adj_vit_start);
    }

    painter << segments;
}

void SkinCurveView::draw(QPainter &painter, const Regular &regular, Regular::Finite_edges_iterator &eit, double shrink_factor)
{
    const Regular::Face_handle &fh = eit->first;
    const int i = eit->second;
    const Weighted_point &wp0 = fh->vertex((i+1)%3)->point();
    const Weighted_point &wp1 = fh->vertex((i+2)%3)->point();

    Vector edge_tangent = wp1-wp0;
    edge_tangent = 1/sqrt(edge_tangent.squared_length()) * edge_tangent;
    Vector edge_normal = edge_tangent.perpendicular(CGAL::CLOCKWISE);

    Weighted_point wp = focus(wp0, wp1);

    double xmin = (1-shrink_factor) * (wp0-wp) * edge_tangent;
    double xmax = (1-shrink_factor) * (wp1-wp) * edge_tangent;
    double ymin, ymax;
    if ((regular.dimension() != 2) || regular.is_infinite(fh))
        ymin = -10000;
    else
        ymin = (regular.dual(fh)-wp) * edge_normal;

    if ((regular.dimension() != 2) || regular.is_infinite(fh->neighbor(i)))
        ymax = 10000;
    else
        ymax = (regular.dual(fh->neighbor(i))-wp) * edge_normal;

    ymin *= shrink_factor;
    ymax *= shrink_factor;

    std::list<Segment> segments;
    generate_hyperbola(-wp.weight(), shrink_factor, xmin, xmax, ymin, ymax, segments);

    position_hyperbola(wp, edge_tangent, edge_normal, segments);
    painter << segments;
}

void SkinCurveView::draw(QPainter &painter, const Regular &/*regular*/, Regular::Finite_faces_iterator &fit, double shrink_factor)
{
    Weighted_point wp = focus(fit);
    wp = Weighted_point(wp, (1-shrink_factor) * wp.weight());

    std::list<Segment> segments;
    generate_circle(wp, segments);

    for (int i=0; i<3; ++i) {
        Line l(wp + (1-shrink_factor)*(fit->vertex(i)->point()-wp),
                        wp + (1-shrink_factor)*(fit->vertex((i+1)%3)->point()-wp));
        clip(segments, l);
    }

    painter << segments;
}


void SkinCurveView::generate_hyperbola(double weight, double shrink_factor,
                double xmin, double xmax, double ymin, double ymax,
                std::list<Segment> &segments)
{
    segments.clear();
    generate_y_positive_hyperbola(weight, shrink_factor, xmin, xmax, -ymax, -ymin, segments);
    BOOST_FOREACH(Segment &s, segments) {
        s = Segment(Bare_point(s[0].x(), -s[0].y()), Bare_point(s[1].x(), -s[1].y()));
    }
    generate_y_positive_hyperbola(weight, shrink_factor, xmin, xmax, ymin, ymax, segments);
}
void SkinCurveView::generate_y_positive_hyperbola(double w0, double s,
                double xmin, double xmax, double ymin, double ymax,
                std::list<Segment> &lst)
{
    if (ymax > 0 && ymax*ymax > s * w0) {
        // we need to draw a part of the positive sheet.
        double xmax_y;
        if (w0 < 0) {
            xmax_y = sqrt((1 - s) * (1/s * ymax * ymax - w0));
        } else {
            if ((1/s * ymax * ymax - w0) < 0) {
                xmax_y = std::max(abs(xmin), abs(xmax));
            } else {
                xmax_y = sqrt((1 - s) * (1/s * ymax * ymax - w0));
            }
        }
        if (ymin < 0 || ymin * ymin < s * w0) {
            // We only need to draw one part.
            // We also have w0 > 0
            // Check whether the range (xmin, xmax) intersects (-xmax_y, xmax_y)
            if (!(xmin > xmax_y || xmax < -xmax_y)) {
                generate_y_positive_hyperbola(w0, s, std::max(xmin, -xmax_y), std::min(xmax, xmax_y), lst);
            }
        } else {
            // draw two parts of the positive sheet
            double xmin_y = sqrt((1 - s) * (1/s * ymin * ymin - w0));
            // Check whether the range (xmin, xmax) intersects (xmin_y, infty)
            if (xmax > xmin_y) {
                generate_y_positive_hyperbola(w0, s, std::max(xmin, xmin_y), std::min(xmax, xmax_y), lst);
            }
            // Check whether the range (xmin, xmax) intersects (infty, -xmin_y)
            if (xmin < -xmin_y) {
                generate_y_positive_hyperbola(w0, s, std::max(xmin, -xmax_y), std::min(-xmin_y, xmax), lst);
            }
        }
    }
}
void SkinCurveView::generate_y_positive_hyperbola(double w0, double s,
                double xmin, double xmax,
                std::list<Segment> &segments)
{
    // Generate the part of the 1 flat, with focus in the origin and weight w0,
    // for which the centres are aligned along the x-axis for which x \in [xmin,xmax]
    double x, y;
    Bare_point p1, p2;

    if (w0 > 0) {
        x = xmin;
        y = sqrt(s*(x*x/(1-s) + w0));
        p1 = Bare_point(x, y);
        x+=stepSize;
        while (x <= xmax) {
            y = sqrt(s*(x*x/(1-s) + w0));
            p2 = Bare_point(x, y);
            segments.push_back(Segment(p1,p2));
            p1 = p2;
            x+=stepSize;
        }

        // draw the final line
        p2 = Bare_point(xmax, sqrt(s*(xmax*xmax/(1-s) + w0)));
        segments.push_back(Segment(p1, p2));
    } else {
        // w0 < 0
        double intersect_x = sqrt(-(1-s) * w0);

        // left part
        if (xmin < -intersect_x) {
            x = xmin;
            y = sqrt(s*(x*x/(1-s) + w0));
            p1 = Bare_point(x, y);
            x+=stepSize;
            while(x <= xmax && x <= -intersect_x) {
                y = sqrt(s*(x*x/(1-s) + w0));
                p2 = Bare_point(x, y);
                segments.push_back(Segment(p1,p2));
                p1 = p2;
                x+=stepSize;
            };
            // draw the final line
            if (xmax < -intersect_x) {
                y = sqrt(s*(xmax*xmax/(1-s) + w0));
                p2 = Bare_point(xmax, y);
            } else {
                p2 = Bare_point(-intersect_x, 0);
            }
            segments.push_back(Segment(p1, p2));
        }

        // right part
        if (intersect_x < xmax) {
            x = xmax;
            y = sqrt(s*(x*x/(1-s) + w0));
            p1 = Bare_point(x, y);
            x -= stepSize;
            while(xmin <= x && intersect_x <= x) {
                y = sqrt(s*(x*x/(1-s) + w0));
                p2 = Bare_point(x, y);
                segments.push_back(Segment(p1,p2));
                p1 = p2;
                x -= stepSize;
            };
            // draw the final line
            if (intersect_x < xmin) {
                y = sqrt(s*(xmin*xmin/(1-s) + w0));
                p2 = Bare_point(xmin, y);
            } else {
                p2 = Bare_point(intersect_x, 0);
            }
            segments.push_back(Segment(p1, p2));
        }
    }
}

void SkinCurveView::position_hyperbola(
                const Bare_point &focus, const Vector &t, const Vector &n,
                std::list<Segment> &segments)
{
    BOOST_FOREACH(Segment &s, segments) {
        Bare_point p0 = focus + t * s[0].x() + n * s[0].y();
        Bare_point p1 = focus + t * s[1].x() + n * s[1].y();
        s = Segment(p0, p1);
    }
}
