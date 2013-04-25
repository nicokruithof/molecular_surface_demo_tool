#include <qt/main_window.h>

#include <iostream>

#include <QFileDialog>

#include <ui_main_window.h>
#include <qt/qt_composite_renderer.h>

#include <osg/screencapture.h>


MainWindow::MainWindow()
  : m_ui(new Ui::MainWindow())
{
  m_ui->setupUi(this);

  m_ui->osg_main_widget->set_scene(m_model.scene());

  QtCompositeRenderer::composite_viewer()->getView(0)->home();
}

MainWindow::~MainWindow() {
  delete(m_ui);
}

void MainWindow::open(const std::string &filename)
{
    m_model.load(filename);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    m_model.update();
    m_ui->osg_main_widget->set_scene(m_model.scene());
}
void MainWindow::on_action_New_triggered() {
//  m_model.clear();
}

void MainWindow::on_actionShow_balls_toggled(bool b)
{
    m_model.show_balls(b);
}

void MainWindow::on_actionShow_skin_surface_toggled(bool b)
{
    // TODO(NGHK): Implement
}

void MainWindow::on_action_Open_triggered() {
  QString fileName = QFileDialog::getOpenFileName(this,
                                                  tr("Open."),
                                                  ".",
                                                  tr("Point data (*.dat)"));
  if(! fileName.isEmpty()) {
      open(fileName.toUtf8().constData());
  }
}

void MainWindow::on_action_Print_triggered() {
  QString fileName = QFileDialog::getSaveFileName(this,
                                                  tr("Save screencapture."),
                                                  ".",
                                                  tr("Image (*.png)"));
  if(! fileName.isEmpty()) {
    ScreenCapture *screen_capture = new ScreenCapture(fileName.toUtf8().constData());

    osg::Camera *cam = QtCompositeRenderer::composite_viewer()->getView(0)->getCamera();
    cam->setFinalDrawCallback(screen_capture);
  }
}
