#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "server.h"
#include "client.h"
#include <QFileDialog>
#include <QThread>
#include "openglwidget.h"
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_listenButton_clicked();
    void on_getButton_clicked();
    void on_openButton_clicked();
    void on_fileDownloaded(QString path);
    void on_dataTransfer();

private:
    Ui::MainWindow *ui;

    OpenGLWidget *m_widget;

    Server *m_server;
    Client *m_client;

    QThread *m_clientThread;

    QTimer *m_errorTimer;
};

#endif // MAINWINDOW_H
