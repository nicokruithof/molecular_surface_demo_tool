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
    std::cout << __PRETTY_FUNCTION__ << std::endl;
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
    if (m_modification_type == CREATE && (m_selected_weighted_point.weight() > 0)) {
        double r = sqrt(m_selected_weighted_point.weight());

        QRectF rectangle(m_selected_weighted_point.x() - r, m_selected_weighted_point.y() - r, 2*r, 2*r);
        painter.drawEllipse(rectangle);
    }
    std::cout << "/" << __PRETTY_FUNCTION__ << std::endl;
}

void MainView::mouseMoveEvent( QMouseEvent * event )
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    Bare_point p = Bare_point(event->pos().x(), event->pos().y());
    double weight = (m_selected_weighted_point-p).squared_length();

    switch (m_modification_type)
    {
    case CREATE: {
        m_selected_weighted_point = Weighted_point(m_selected_weighted_point, weight);
        break;
    }
    case MODIFY_WEIGHT: {
        m_model->remove(m_selected_weighted_point);
        m_selected_weighted_point = Weighted_point(m_selected_weighted_point, weight);
        m_model->insert(m_selected_weighted_point);
        break;
    }
    case MODIFY_POSITION: {
        m_model->remove(m_selected_weighted_point);
        m_selected_weighted_point = Weighted_point(p + m_center_offset_vector, m_selected_weighted_point.weight());
        m_model->insert(m_selected_weighted_point);
        break;
    }
    }
    update();
    std::cout << "/" << __PRETTY_FUNCTION__ << std::endl;
}
void MainView::mousePressEvent( QMouseEvent * event )
{
    Bare_point c(event->pos().x(), event->pos().y());

    m_modification_type = NONE;

    std::cout << __PRETTY_FUNCTION__ << std::endl;
    if (event->modifiers() == 0) {
        m_modification_type = CREATE;
        m_selected_weighted_point = c;
    } else {
        if (event->modifiers() == Qt::ControlModifier)
            m_modification_type = MODIFY_WEIGHT;
        else
            m_modification_type = MODIFY_POSITION;

        const std::vector<Weighted_point> &pts = m_model->points();
        if (m_modification_type == NONE || pts.empty()) {
            m_modification_type = NONE;
        } else {
            // Find the closest weighted point
            m_selected_weighted_point = *pts.begin();
            double d = std::abs(sqrt((m_selected_weighted_point-c).squared_length())-sqrt(m_selected_weighted_point.weight()));

            BOOST_FOREACH(const Weighted_point &wp, pts) {
                double d2 = std::abs(sqrt((wp-c).squared_length())-sqrt(wp.weight()));
                if (d2 < d)
                {
                    m_selected_weighted_point = wp;
                    d = d2;
                }
            }

            m_center_offset_vector = m_selected_weighted_point-c;

            // Remove point
            if (event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier)) {
                m_model->remove(m_selected_weighted_point);
                m_modification_type = NONE;
            }
        }
    }

    std::cout << "/" << __PRETTY_FUNCTION__ << std::endl;

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
        m_model->insert(m_selected_weighted_point);
        break;
    }
    case MODIFY_WEIGHT:
    case MODIFY_POSITION: {
        break;
    }
    }
    m_modification_type = NONE;
    update();
}
