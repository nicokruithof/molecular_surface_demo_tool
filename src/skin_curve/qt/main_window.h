#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtGui/QMainWindow>

#include <string>
#include <model/model.h>

#include <view/delaunay_view.h>
#include <view/skin_curve_view.h>
#include <view/voronoi_view.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    void draw(QPainter &painter);

private slots:
    void on_actionShow_circles_toggled(bool);
    void on_actionShow_Voronoi_toggled(bool);
    void on_actionShow_Delaunay_toggled(bool);
    void on_actionShow_skin_curve_toggled(bool);

    void on_shrink_factor_slider_valueChanged();
    void on_shrink_factor_spinbox_valueChanged();

private:

/// User interface
Ui::MainWindow       *m_ui;

Model                m_model;

DelaunayView         m_delaunay_view;
VoronoiView          m_voronoi_view;
SkinCurveView        m_skin_curve_view;
};

#endif // MAINWINDOW
