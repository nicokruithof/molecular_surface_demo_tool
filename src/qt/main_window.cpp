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

//  m_model.show_vertices(m_ui->actionShowVertices->isChecked());
//  m_model.show_edges(m_ui->actionShowEdges->isChecked());
//  m_model.show_faces(m_ui->actionShowFaces->isChecked());

  // m_model.open("../periodic_data_sets/nico-1_200.dat");
  on_action_Initialize_random_points_triggered();

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

void MainWindow::on_action_Initialize_random_points_triggered() {
//  m_model.initialize_random(2000);
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

void MainWindow::on_actionShowVertices_toggled(bool b) {
//  m_model.show_vertices(b);
}

void MainWindow::on_actionShowEdges_toggled(bool b) {
//  m_model.show_edges(b);
}

void MainWindow::on_actionShowFaces_toggled(bool b) {
//  m_model.show_faces(b);
}
