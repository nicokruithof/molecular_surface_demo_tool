#ifndef VIEW_MAIN_VIEW_H
#define VIEW_MAIN_VIEW_H

#include <QtGui>
#include <QtGui/QWidget>

#include <boost/shared_ptr.hpp>

#include <model/model.h>
#include <controller/controller.h>

class MainView : public QWidget {
  Q_OBJECT

public:
  MainView(QWidget * parent = 0, Qt::WindowFlags f = 0);
  void set_model(Model *model);

  virtual void paintEvent(QPaintEvent *event);

  virtual void mouseMoveEvent( QMouseEvent * event );
  virtual void mousePressEvent( QMouseEvent * event );
  virtual void mouseReleaseEvent( QMouseEvent * event );

private:
  Model *m_model;
  Controller m_controller;

private:
  enum ModificationType {
      NONE,
      CREATE,
      MODIFY_WEIGHT,
      MODIFY_POSITION
  } m_modification_type;

  Weighted_point m_selected_weighted_point;
  Vector m_center_offset_vector;
};

#endif // VIEW_MAIN_VIEW_H
