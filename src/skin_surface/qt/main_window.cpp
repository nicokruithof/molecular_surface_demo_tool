#include <qt/main_window.h>

#include <math.h>

#include <iostream>

#include <boost/foreach.hpp>

#include <QFileDialog>

#include <ui_main_window.h>
#include <ui_statistics.h>

#include <qt/qt_composite_renderer.h>

#include <osg/screencapture.h>

#include <utils/statistic.h>


MainWindow::MainWindow()
  : m_ui(new Ui::MainWindow())
  , m_statistics_ui(new Ui::StatisticsDialog())
  , m_statistics_dialog(new QDialog())
{
  m_ui->setupUi(this);
  m_statistics_ui->setupUi(m_statistics_dialog);

  m_ui->osg_main_widget->set_scene(m_model.scene());

  on_actionShow_balls_toggled(m_ui->actionShow_balls->isChecked());
  on_actionShow_skin_surface_toggled(m_ui->actionShow_skin_surface->isChecked());
  on_actionColor_skin_surface_triggered(m_ui->actionColor_skin_surface->isChecked());
}

MainWindow::~MainWindow() {
    delete(m_ui);
}

void MainWindow::open(const std::string &filename)
{
    m_model.load(filename);
    m_model.update();
    QtCompositeRenderer::composite_viewer()->getView(0)->home();
}

void MainWindow::paintEvent(QPaintEvent * /*event*/)
{
    m_model.update();
    m_ui->osg_main_widget->set_scene(m_model.scene());
}
void MainWindow::on_action_New_triggered() {
  m_model.clear();
}

void MainWindow::on_actionShow_balls_toggled(bool b)
{
    m_model.show_balls(b);
}

void MainWindow::on_actionShow_skin_surface_toggled(bool b)
{
    m_model.show_skin_surface(b);
}
void MainWindow::on_actionSubdivide_skin_surface_triggered(bool /*checked*/)
{
    m_model.subdivide_skin_surface_mesh();
    m_model.update();
}
void MainWindow::on_actionColor_skin_surface_triggered(bool checked)
{
    m_model.color_skin_surface(checked);
}
void MainWindow::on_actionShow_statistics_triggered(bool /*checked*/)
{
    m_statistics_dialog->hide();

    std::list<Statistic> stats;
    m_model.get_statistics(stats);

    QTreeWidget *tree_widget = m_statistics_ui->statistics;
    tree_widget->clear();

    QTreeWidgetItem *main_item = NULL;
    std::string category = "";
    BOOST_FOREACH(Statistic &stat, stats) {
        if (stat.category != category) {
            category = stat.category;

            main_item = new QTreeWidgetItem();
            tree_widget->addTopLevelItem(main_item);

            main_item->setData(0, Qt::DisplayRole, QString(stat.category.c_str()));
            main_item->setExpanded(true);
        }
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setData(0, Qt::DisplayRole, QString(stat.name.c_str()));
        QString val(stat.value.c_str());
        if (stat.unit != "")
            val += QString(("["+stat.unit+"]").c_str());
        item->setData(1, Qt::DisplayRole, val);
        main_item->addChild(item);
    }

    m_statistics_dialog->show();
    m_statistics_dialog->raise();
    m_statistics_dialog->activateWindow();
}


void MainWindow::on_action_Open_triggered() {
  QString fileName = QFileDialog::getOpenFileName(this,
                                                  tr("Open."),
                                                  ".",
                                                  tr("Weighted points (*.cin);;Proteine data base (*.pdb)"));
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

void MainWindow::on_shrink_factor_slider_valueChanged() {
    double shrink_factor = m_ui->shrink_factor_slider->value()*1.0/m_ui->shrink_factor_slider->maximum();
    m_model.set_shrink_factor(shrink_factor);
    m_ui->shrink_factor_spinbox->setValue(shrink_factor);
}

void MainWindow::on_subdivision_slider_valueChanged() {
    int subdivision = static_cast<int>(m_ui->subdivision_slider->value());
    m_model.set_subdivision(subdivision);
}

void MainWindow::on_shrink_factor_spinbox_valueChanged()
{
    double shrink_factor = m_ui->shrink_factor_spinbox->value();
    m_model.set_shrink_factor(shrink_factor);
    m_ui->shrink_factor_slider->setValue(m_ui->shrink_factor_slider->maximum()*shrink_factor);
}
