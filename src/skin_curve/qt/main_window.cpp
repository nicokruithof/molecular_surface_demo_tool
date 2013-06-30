#include <qt/main_window.h>

#include <iostream>
#include <fstream>

#include <boost/foreach.hpp>

#include <QFileDialog>

#include <ui_main_window.h>
#include <view/utils.h>

#define LOG(msg)
#define LOG_VERBOSE(msg)
//#define LOG(msg) std::cout << msg << std::endl;
//#define LOG_VERBOSE(msg) std::cout << msg << std::endl;

MainWindow::MainWindow()
: m_ui(new Ui::MainWindow())
, m_color_skin(false)
{
    m_ui->setupUi(this);
    m_ui->main_view->set_model(&m_model);

    m_model.set_multiply_with_shrink_factor(m_ui->multiply_with_shrink_button->isChecked());
}

MainWindow::~MainWindow() {
    delete(m_ui);
}

void MainWindow::load(const char *filename) {
    std::ifstream file(filename);
    Weighted_point wp;

    m_model.clear();
//    double scale = 80;
//    Vector offset(600, 335);
    while (file >> wp) {
//        wp = Weighted_point(Bare_point(scale*wp.y(), scale*wp.x())+offset, scale*scale*wp.weight());
//        std::cout << wp << std::endl;
        m_model.insert(wp);
    }
}

void MainWindow::draw(QPainter &painter)
{
    LOG(__PRETTY_FUNCTION__);

    if (m_ui->actionShow_atoms->isChecked()) {
        LOG_VERBOSE("circles");
        QPen pen;
        pen.setWidth(3);
        painter.setPen(pen);

        const std::vector<Weighted_point> &pts = m_model.points();
        BOOST_FOREACH(Weighted_point wp, pts) {
            painter << wp;
//            if (pt.weight() > 0) {
//                double r = sqrt(pt.weight());
//                QRectF rectangle(pt.x()-r, pt.y()-r, 2*r, 2*r);
//
//                painter.drawEllipse(rectangle);
//            }
        }
        LOG_VERBOSE("/circles");
    }
    if (m_ui->actionShow_circles->isChecked()) {
        LOG_VERBOSE("regular circles");
        QPen pen;
        pen.setWidth(1);
        painter.setPen(pen);
        const Regular &regular = m_model.regular();
        for (Regular::Finite_vertices_iterator vit = regular.finite_vertices_begin();
                        vit != regular.finite_vertices_end(); ++vit) {
            painter << vit->point();
//            if (pt.weight() > 0) {
//                double r = sqrt(pt.weight());
//                QRectF rectangle(pt.x()-r, pt.y()-r, 2*r, 2*r);
//
//                painter.drawEllipse(rectangle);
//            }
        }
        LOG_VERBOSE("/regular circles");
    }


    if (m_ui->actionShow_Voronoi->isChecked()) {
        LOG_VERBOSE("voronoi");
        m_voronoi_view.draw(painter, m_model.regular());
        LOG_VERBOSE("/voronoi");
    }

    if (m_ui->actionShow_Delaunay->isChecked()) {
        LOG_VERBOSE("delaunay");
        m_delaunay_view.draw(painter, m_model.regular());
        LOG_VERBOSE("/delaunay");
    }

    if (m_ui->actionShow_skin_curve->isChecked()) {
        LOG_VERBOSE("skin");
        m_skin_curve_view.draw(painter, m_model.regular(), m_model.shrink_factor(), m_color_skin);
        LOG_VERBOSE("/skin");
    }

    if (m_ui->actionShow_union->isChecked()) {
        LOG_VERBOSE("union");
        m_union_of_balls_view.draw(painter, m_model.regular());
        LOG_VERBOSE("/union");
    }

    if (m_ui->actionShow_mixed_complex->isChecked()) {
        LOG_VERBOSE("mixed");
        m_mixed_complex_view.draw(painter, m_model.regular(), m_model.shrink_factor());
        LOG_VERBOSE("/mixed");
    }
    LOG("/" << __PRETTY_FUNCTION__);
}

void MainWindow::on_action_New_triggered() {
    m_model.clear();
    m_ui->main_view->update();
}
void MainWindow::on_action_Print_triggered() {
  QString fileName = "/home/nico/Code/skin_surface_viewer_build/test.png";
//QFileDialog::getSaveFileName(this,
//                                                  tr("Save screencapture."),
//                                                  ".",
//                                                  tr("Image (*.png)"));
  if(! fileName.isEmpty()) {
      std::string current_string = fileName.toLocal8Bit().constData();
      std::cout << current_string << std::endl;
      //current_string = current_string + ".png";

      char * buffer = new char[current_string.length()];
      std::string temp = buffer;
      char* temp2 = &temp[0];
      strcpy(buffer, current_string.c_str());

      char* pch = strtok (temp2,".");
      pch = strtok (NULL, ".");

      if(!QPixmap::grabWindow(m_ui->main_view->winId()).save(buffer,pch))
      {
          QMessageBox::warning(this, "File could not be saved", "ok", QMessageBox::Ok);
      }
  }
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
void MainWindow::on_probe_radius_slider_valueChanged()
{
    m_model.set_probe_radius(m_ui->probe_radius_slider->value());
    m_ui->main_view->update();
}
void MainWindow::on_actionShow_atoms_toggled(bool) {
    m_ui->main_view->update();
}
void MainWindow::on_actionShow_circles_toggled(bool) {
    m_ui->main_view->update();
}
void MainWindow::on_actionShow_Voronoi_toggled(bool) {
    m_ui->main_view->update();
}
void MainWindow::on_actionShow_Delaunay_toggled(bool) {
    m_ui->main_view->update();
}
void MainWindow::on_actionShow_union_toggled(bool) {
    m_ui->main_view->update();
}
void MainWindow::on_actionShow_skin_curve_toggled(bool) {
    m_ui->main_view->update();
}
void MainWindow::on_actionShow_mixed_complex_toggled(bool) {
    m_ui->main_view->update();
}
void MainWindow::on_actionColor_skin_curve_toggled(bool b) {
    m_color_skin = b;
    m_ui->main_view->update();
}

void MainWindow::on_multiply_with_shrink_button_clicked() {
    m_model.set_multiply_with_shrink_factor(m_ui->multiply_with_shrink_button->isChecked());
    m_ui->main_view->update();
}
