#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtGui/QMainWindow>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
  
public:
  MainWindow();
  ~MainWindow();
               
private slots:
  void paintEvent(QPaintEvent *event);

private:

  /// User interface
  Ui::MainWindow       *m_ui;

  // Model                m_model;
};

#endif // MAINWINDOW
