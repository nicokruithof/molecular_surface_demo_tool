#include <view/utils.h>

void operator<<(QPainter &painter, const Segment &s)
{
    painter.drawLine(s[0][0], s[0][1], s[1][0], s[1][1]);
}

void operator<<(QPainter &painter, const Ray &r)
{
    const Bare_point &s = r.start();
    Vector d = r.direction().to_vector();
    double len = d.squared_length();
    double window_dim = std::max(painter.window().width(), painter.window().height());
    d = d * window_dim/std::min(len, 1.0);
    painter << Segment(s, s+d);
}

void operator<<(QPainter &painter, const Line &l)
{
    const Bare_point &s = l.point();
    Vector d = l.direction().to_vector();
    double len = d.squared_length();
    double window_dim = std::max(painter.window().width(), painter.window().height());
    d = d * window_dim/std::min(len, 1.0);
    painter << Segment(s-d, s+d);
}

