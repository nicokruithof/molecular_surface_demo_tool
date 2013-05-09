#ifndef VIEW_MAIN_VIEW_H
#define VIEW_MAIN_VIEW_H

#include <QtGui>
#include <QtGui/QWidget>

#include <boost/shared_ptr.hpp>

#include <model/model.h>
#include <controller/controller.h>

class MainView : public QWidget {
  Q_OBJECT;

public:
  MainView(QWidget * parent = 0, Qt::WindowFlags f = 0);

  virtual void paintEvent(QPaintEvent *event);

  virtual void mouseMoveEvent( QMouseEvent * event );
  virtual void mousePressEvent( QMouseEvent * event );
  virtual void mouseReleaseEvent( QMouseEvent * event );

private:
  boost::shared_ptr<Model> m_model;
  boost::shared_ptr<Controller> m_controller;

private:
  QPoint m_circle_center;
  double m_radius;
};

#endif // VIEW_MAIN_VIEW_H
