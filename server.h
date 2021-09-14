#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDataStream>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0, QString ip = "127.0.0.1", quint32 port = 12345, quint32 sendPort = 12346, QString path = "" );
    ~Server();

private:
    QString m_ip;
    quint32 m_port;
    quint32 m_sendPort;
    QString m_filePath;

    QUdpSocket *m_sendSocket;
    QUdpSocket *m_receiveSocket;

signals:

public slots:
    void readPendingDatagrams();
};

#endif // SERVER_H
