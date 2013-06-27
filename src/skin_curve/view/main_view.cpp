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
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    QWidget *p = parentWidget();
    while (p != NULL) {
        MainWindow *main_window = dynamic_cast<MainWindow *>(p);
        if (main_window != NULL)
            main_window->draw(painter);
        p = p->parentWidget();
    }

    QPen pen;
    painter.setPen(pen);
    if (m_modification_type == CREATE) {
        QRectF rectangle(m_circle_center.x()-m_radius, m_circle_center.y()-m_radius,
                        2*m_radius, 2*m_radius);

        painter.drawEllipse(rectangle);
    }
}

void MainView::mouseMoveEvent( QMouseEvent * event )
{
    switch (m_modification_type)
    {
    case CREATE: {
        QPoint vec = event->pos() - m_circle_center;
        m_radius = sqrt(vec.x()*vec.x() + vec.y()*vec.y());
        break;
    }
    case MODIFY: {
        if (m_vh != NULL) {
            double weight = (m_vh->point()-Bare_point(event->pos().x(), event->pos().y())).squared_length();
            Weighted_point wp(m_vh->point(), weight);
            m_model->regular().remove(m_vh);
            m_model->regular().insert(wp);
        }
        break;
    }
    }
    update();
}
void MainView::mousePressEvent( QMouseEvent * event )
{
    if ((event->modifiers() & Qt::ControlModifier) != 0) {
        m_modification_type = MODIFY;

        Bare_point c(event->pos().x(), event->pos().y());

        // Find the closest weighted point
        m_vh = m_model->regular().finite_vertices_begin();
        double d = std::abs(sqrt((m_vh->point()-c).squared_length())-sqrt(m_vh->point().weight()));

        Regular::Finite_vertices_iterator vit;
        for (vit = m_model->regular().finite_vertices_begin(); vit != m_model->regular().finite_vertices_end(); ++vit) {
            double d2 = std::abs(sqrt((vit->point()-c).squared_length())-sqrt(vit->point().weight()));
            if (d2 < d)
            {
                m_vh = vit;
                d = d2;
            }
        }
    }
    else
    {
        m_modification_type = CREATE;

        m_circle_center = event->pos();
        m_radius = 0;
    }
}
void MainView::set_model(Model* model)
{
    m_model = model;
    m_controller.set_model(model);

}

void MainView::mouseReleaseEvent( QMouseEvent * /*event*/ )
{
    switch (m_modification_type) {
    case CREATE: {
        m_controller.insert(m_circle_center.x(), m_circle_center.y(), m_radius*m_radius);
        m_radius = -1;
        break;
    }
    case MODIFY: {
        m_vh = NULL;
        break;
    }
    }
    update();
}
