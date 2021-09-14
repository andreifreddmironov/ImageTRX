#include "openglwidget.h"

OpenGLWidget::OpenGLWidget()
{
    qDebug() << Q_FUNC_INFO;
}

void OpenGLWidget::display(const QImage& image)
{
    m_image = image;
    this->update();
}

void OpenGLWidget::paintEvent(QPaintEvent*e)
{
    Q_UNUSED(e)
    QPainter painter(this);
    painter.drawImage(this->rect(), m_image);
}
