#ifndef VIEW_MIXED_COMPLEX_VIEW_H
#define VIEW_MIXED_COMPLEX_VIEW_H

#include <qpainter.h>

#include <model/model.h>

class MixedComplexView {
public:
    void draw(QPainter &painter, Regular &regular, double s);
};

#endif // VIEW_MIXED_COMPLEX_VIEW_H
