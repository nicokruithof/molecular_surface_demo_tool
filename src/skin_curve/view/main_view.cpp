#include <view/main_view.h>
#include <qt/main_window.h>

#include <iostream>
#include <boost/foreach.hpp>

MainView::MainView(QWidget * parent, Qt::WindowFlags f)
: QWidget(parent, f)
, m_model(NULL)
{
}


void MainView::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QWidget *p = parentWidget();
    while (p != NULL) {
        MainWindow *main_window = dynamic_cast<MainWindow *>(p);
        if (main_window != NULL)
            main_window->draw(painter);
        p = p->parentWidget();
    }

    QPen pen;
    painter.setPen(pen);
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
void MainView::set_model(Model* model)
{
    m_model = model;
    m_controller.set_model(model);

}

void MainView::mouseReleaseEvent( QMouseEvent * /*event*/ )
{
    m_controller.insert(m_circle_center.x(), m_circle_center.y(), m_radius*m_radius);
    update();
}
