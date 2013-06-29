#ifndef VIEW_VORONOI_VIEW_H
#define VIEW_VORONOI_VIEW_H

#include <qpainter.h>

#include <model/model.h>

class VoronoiView {
public:
    void draw(QPainter &painter, const Regular &regular);
};

#endif // VIEW_VORONOI_VIEW_H
