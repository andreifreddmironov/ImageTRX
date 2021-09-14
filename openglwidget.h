#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QObject>
#include <QPainter>
#include <QDebug>
#include <QOpenGLFunctions>

class OpenGLWidget : public QOpenGLWidget
{
public:
    OpenGLWidget();

    void display(const QImage& image);
    void paintEvent(QPaintEvent *e) override;

private:
    QImage m_image;
};

#endif // OPENGLWIDGET_H
