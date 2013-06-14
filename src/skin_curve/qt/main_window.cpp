#include <qt/main_window.h>

#include <iostream>

#include <boost/foreach.hpp>

#include <QFileDialog>

#include <ui_main_window.h>

MainWindow::MainWindow()
  : m_ui(new Ui::MainWindow())
{
  m_ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete(m_ui);
}

void MainWindow::paintEvent(QPaintEvent * /*event*/)
{
    // m_model.update();
    // m_ui->osg_main_widget->set_scene(m_model.scene());
}
