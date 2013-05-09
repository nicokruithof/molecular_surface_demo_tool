#ifndef VIEW_MAIN_VIEW_H
#define VIEW_MAIN_VIEW_H

#include <QtGui>
#include <QtGui/QWidget>

#include <boost/shared_ptr.hpp>
#include <model/model.h>

#include <iostream>

class MainView : public QWidget {
  Q_OBJECT;

public:
  MainView(QWidget * parent = 0, Qt::WindowFlags f = 0);

void paintEvent(QPaintEvent *event)
{
      QPainter painter(this);
      
      //First Circle:
      //width&height:150px
      //Beginning(x/y):50px/30px
      //Each step for Angle parameter is 1/16th of a Degree
      //so multiply Angle Value with 16
      painter.drawArc(50,30,150,150,0,16*360);
      painter.drawArc(200,80,100,100,0,16*360);
      painter.drawArc(300,130,50,50,0,16*360);

 QRectF rectangle(10.0, 20.0, 80.0, 60.0);

 painter.drawEllipse(rectangle);
  std::cout << __FUNCTION__ << std::endl;
    // m_model.update();
    // m_ui->osg_main_widget->set_scene(m_model.scene());
}

private:
  boost::shared_ptr<Model> m_model;
};

#endif // VIEW_MAIN_VIEW_H
