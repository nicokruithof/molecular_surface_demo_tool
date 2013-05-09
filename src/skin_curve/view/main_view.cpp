#include <view/main_view.h>

#include <iostream>

MainView::MainView(QWidget * parent, Qt::WindowFlags f)
  : QWidget(parent, f)
{
}


void MainView::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  if (m_radius > 0) {
    QRectF rectangle(m_circle_center.x()-m_radius, m_circle_center.y()-m_radius,
                     2*m_radius, 2*m_radius);

    painter.drawEllipse(rectangle);
  }
}

void MainView::mouseMoveEvent( QMouseEvent * event )
{
  QPoint vec = event->pos() - m_circle_center;
  m_radius = sqrt(pow(vec.x(), 2) + pow(vec.y(), 2));

  update();
}
void MainView::mousePressEvent( QMouseEvent * event )
{
  m_circle_center = event->pos();
  m_radius = 0;
}
void MainView::mouseReleaseEvent( QMouseEvent * event )
{
  if (m_controller)
    m_controller->insert(m_circle_center.x(), m_circle_center.y(), m_radius*m_radius);
}
