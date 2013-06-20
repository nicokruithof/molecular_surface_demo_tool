#include <qt/main_window.h>

#include <iostream>

#include <boost/foreach.hpp>

#include <QFileDialog>

#include <ui_main_window.h>

MainWindow::MainWindow()
  : m_ui(new Ui::MainWindow())
{
  m_ui->setupUi(this);
  m_ui->main_view->set_model(&m_model);
}

MainWindow::~MainWindow() {
    delete(m_ui);
}

void MainWindow::draw(QPainter &painter)
{
    std::vector<Weighted_point> pts;
    m_model.get_points(pts);
    BOOST_FOREACH(Weighted_point pt, pts) {
        if (pt.weight() > 0) {
            double r = sqrt(pt.weight());
            QRectF rectangle(pt.x()-r, pt.y()-r, 2*r, 2*r);

            painter.drawEllipse(rectangle);
        }
    }

    m_skin_curve_view.draw(painter, m_model.regular(), m_model.shrink_factor());
    m_voronoi_view.draw(painter, m_model.regular());
}

void MainWindow::on_shrink_factor_slider_valueChanged()
{
    double shrink_factor = m_ui->shrink_factor_slider->value()*1.0/m_ui->shrink_factor_slider->maximum();
    m_model.set_shrink_factor(shrink_factor);
    m_ui->shrink_factor_spinbox->setValue(shrink_factor);
    m_ui->main_view->update();
}
void MainWindow::on_shrink_factor_spinbox_valueChanged()
{
    double shrink_factor = m_ui->shrink_factor_spinbox->value();
    m_model.set_shrink_factor(shrink_factor);
    m_ui->shrink_factor_slider->setValue(m_ui->shrink_factor_slider->maximum()*shrink_factor);
    m_ui->main_view->update();
}
