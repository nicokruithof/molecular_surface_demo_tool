#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtGui/QMainWindow>
#include <model/model.h>
#include <string>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT;
  
public:
  MainWindow();
  ~MainWindow();

  void open(const std::string &filename);
               
private slots:
  void on_action_New_triggered();
  void on_action_Open_triggered();
  void on_action_Print_triggered();
  void on_action_Initialize_random_points_triggered();
  void on_actionShowVertices_toggled(bool b);
  void on_actionShowEdges_toggled(bool b);
  void on_actionShowFaces_toggled(bool b);

  void on_action_Iteration_triggered();

private:

  /// User interface
  Ui::MainWindow *m_ui;
  Model           m_model;
};

#endif // MAINWINDOW
