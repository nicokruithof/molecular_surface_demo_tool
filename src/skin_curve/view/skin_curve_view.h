#ifndef VIEW_SKIN_CURVE_VIEW_H
#define VIEW_SKIN_CURVE_VIEW_H

#include <qpainter.h>

#include <view/union_of_balls_view.h>

#include <model/model.h>

class SkinCurveView : public UnionOfBallsView {
public:
    void draw(QPainter &painter, Regular &regular, double shrink_factor);

private:
    void draw(QPainter &painter, Regular &regular, Regular::Finite_vertices_iterator &vit, double shrink_factor);
    void draw(QPainter &painter, Regular &regular, Regular::Finite_edges_iterator &eit, double shrink_factor);
    void draw(QPainter &painter, Regular &regular, Regular::Finite_faces_iterator &vit, double shrink_factor);

protected:
    double stepSize;

    // Generates an hyperbola
    void generate_hyperbola(double weight, double shrink_factor,
                    double xmin, double xmax, double ymin, double ymax,
                    std::list<Segment> &segments);
    void generate_y_positive_hyperbola(double weight, double shrink_factor,
                    double xmin, double xmax, double ymin, double ymax,
                    std::list<Segment> &segments);
    void generate_y_positive_hyperbola(double weight, double shrink_factor,
                    double xmin, double xmax,
                    std::list<Segment> &segments);
    void position_hyperbola(const Bare_point &focus, const Vector &t, const Vector &n, std::list<Segment> &segments);

};

#endif // VIEW_SKIN_CURVE_VIEW_H
