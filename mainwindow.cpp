#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_server(nullptr),
    m_client(nullptr),
    m_clientThread(nullptr)
{
    ui->setupUi(this);
    m_widget = new OpenGLWidget();

    m_errorTimer = new QTimer(this);
    connect(m_errorTimer, &QTimer::timeout, [=](){QMessageBox msgBox;
        msgBox.setText("Transfer  failed!");
        m_errorTimer->stop();
        msgBox.exec();});
}

MainWindow::~MainWindow()
{
    if(m_server)
    {
        delete m_server;
    }
    if(m_clientThread)
    {
        m_clientThread->quit();
        m_clientThread->wait();
        delete m_clientThread;
    }
    if(m_widget)
    {
        delete m_widget;
    }
    delete ui;
}

void MainWindow::on_listenButton_clicked()
{
    if(ui->listenButton->text() == "Listen")
    {
        ui->listenButton->setText("Listening...");
        ui->ipServerLineEdit->setEnabled(false);
        ui->portServerLineEdit->setEnabled(false);
        ui->sendPortServerLineEdit->setEnabled(false);
        m_server = new Server( nullptr, ui->ipServerLineEdit->text(),
                               ui->portServerLineEdit->text().toInt(),
                               ui->sendPortServerLineEdit->text().toInt(),
                               ui->pathLineEdit->text());
    }else
    {
        ui->listenButton->setText("Listen");
        ui->ipServerLineEdit->setEnabled(true);
        ui->portServerLineEdit->setEnabled(true);
        ui->sendPortServerLineEdit->setEnabled(true);
        if(m_server)
        {
            delete m_server;
            m_server = nullptr;
        }
    }
}

void MainWindow::on_getButton_clicked()
{
    if(m_clientThread)
    {
        m_clientThread->quit();
        m_clientThread->wait();
        delete m_clientThread;
    }

    m_client = new Client( nullptr, ui->ipClientLineEdit->text(),
                          ui->portClientLineEdit->text().toInt(),
                          ui->receivePortClientLineEdit->text().toInt());
    m_clientThread = new QThread(this);


    m_client->moveToThread(m_clientThread);
    connect(m_clientThread, &QThread::finished, m_client, &Client::deleteLater);

    connect(m_client, &Client::fileDownloaded, this, &MainWindow::on_fileDownloaded);
    connect(m_client, &Client::dataTransfer, this, &MainWindow::on_dataTransfer);
    m_clientThread->start();
}

void MainWindow::on_openButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Open file", "", "Image file (*.bmp )");
    ui->pathLineEdit->setText(path);
}

void MainWindow::on_fileDownloaded(QString path)
{
    m_errorTimer->stop();
    QImage img(path);
    m_widget->display(img);
    m_widget->show();
}

void MainWindow::on_dataTransfer()
{
    m_errorTimer->start(2000);
}

