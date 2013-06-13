#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtGui/QMainWindow>
#include <model/model.h>
#include <string>

namespace Ui {
class MainWindow;
class StatisticsDialog;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
  
public:
  MainWindow();
  ~MainWindow();

  void open(const std::string &filename);
               
private slots:
  void paintEvent(QPaintEvent *event);

  void on_action_New_triggered();
  void on_action_Open_triggered();
  void on_action_Print_triggered();

  void on_actionShow_statistics_triggered(bool checked = false);

  void on_actionShow_balls_toggled(bool b);
  void on_actionShow_skin_surface_toggled(bool b);
  void on_actionSubdivide_skin_surface_triggered(bool checked = false);
  void on_actionColor_skin_surface_triggered(bool checked);

  void on_shrink_factor_slider_valueChanged();
  void on_shrink_factor_spinbox_valueChanged();

  void on_subdivision_slider_valueChanged();
private:

  /// User interface
  Ui::MainWindow       *m_ui;
  Ui::StatisticsDialog *m_statistics_ui;
  QDialog              *m_statistics_dialog;

  Model                m_model;
};

#endif // MAINWINDOW
