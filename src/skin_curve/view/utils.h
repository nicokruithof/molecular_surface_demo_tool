#ifndef VIEW_UTILS_H
#define VIEW_UTILS_H

#include <qpainter.h>
#include <model/model.h>

void operator<<(QPainter &painter, const Segment &s);
void operator<<(QPainter &painter, const Ray &r);
void operator<<(QPainter &painter, const Line &l);
void operator<<(QPainter &painter, const std::list<Segment> &segments);

#endif // VIEW_UTILS_H
