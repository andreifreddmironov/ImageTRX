#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDataStream>
#include <QList>
#include <QBuffer>
#include <QTimer>
#include <QFile>
#include <QTemporaryDir>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0, QString ip = "127.0.0.1", quint32 port = 12345, quint32 receivePort = 12346);
    ~Client();

private:
    QString m_ip;
    quint32 m_port;
    quint32 m_receivePort;

    quint32 m_bytesReceived;
    quint32 m_packetsQuantityWaiting;
    quint32 m_writePacketsCounter;

    QList<QByteArray> m_dataList;

    QByteArray m_buffer;

    QUdpSocket *m_sendSocket;
    QUdpSocket *m_receiveSocket;
    QTimer *m_parseTimer;


    void sendStart();
    void parseBuffer();

signals:
    void fileDownloaded(QString);
    void dataTransfer();

public slots:
    void readPendingDatagrams();
};

#endif // CLIENT_H
