#ifndef VIEW_DELAUNAY_VIEW_H
#define VIEW_DELAUNAY_VIEW_H

#include <qpainter.h>

#include <model/model.h>

class DelaunayView {
public:
    void draw(QPainter &painter, const Regular &regular);
};

#endif // VIEW_DELAUNAY_VIEW_H
