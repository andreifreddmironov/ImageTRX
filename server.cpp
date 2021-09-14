#include "server.h"

Server::Server(QObject *parent, QString ip, quint32 port, quint32 sendPort, QString path) : QObject(parent),
    m_ip(ip),
    m_port(port),
    m_sendPort(sendPort),
    m_filePath(path),
    m_sendSocket(nullptr),
    m_receiveSocket(nullptr)
{
    m_receiveSocket = new QUdpSocket(this);
    if(m_receiveSocket)
    {
        if(!m_receiveSocket->bind(QHostAddress(ip), port))
        {
            qDebug() << "Error binding";
        }

        connect(m_receiveSocket, SIGNAL(readyRead()),
                this, SLOT(readPendingDatagrams()));
    }else
    {
        qDebug() << "Error while creating receive socket!";
    }

    m_sendSocket = new QUdpSocket(this);
    if(m_sendSocket)
    {
        m_sendSocket->connectToHost(QHostAddress(ip), m_sendPort);
    }else
    {
        qDebug() << "Error while creating send socket!";
    }
}

Server::~Server()
{
    if(m_sendSocket)
    {
        delete m_sendSocket;
    }
    if(m_receiveSocket)
    {
        delete m_receiveSocket;
    }
}

void Server::readPendingDatagrams()
{
    while (m_receiveSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_receiveSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_receiveSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        QJsonParseError parseError;
        QJsonDocument jsonDoc;
        jsonDoc = QJsonDocument::fromJson(datagram, &parseError);
        if(parseError.error != QJsonParseError::NoError){
            qWarning() << "Parse error at " << parseError.offset << ":" << parseError.errorString();
            return;
        }

        QJsonObject jsonObj;
        jsonObj = jsonDoc.object();

        if(jsonObj.value("command").toString() == "start")
        {
            QFile imageFile(m_filePath);
            if(imageFile.open(QFile::ReadWrite))
            {
                quint32 size = imageFile.size();
                quint32 packetsCounter = 0;
                QByteArray readed = "0";
                while(readed.size() > 0)
                {
                    readed = imageFile.read(200);
                    packetsCounter++;

                    QByteArray data;
                    QDataStream stream(&data, QIODevice::WriteOnly);

                    stream << (quint8)0x01;//type - 1 byte
                    stream << (quint32)size;//full size - 4 bytes
                    stream << (quint32)packetsCounter;//packet number - 4 bytes
                    stream << (quint16)readed.size();//packet size
                    stream << readed;//packet size
                    m_sendSocket->writeDatagram(data, QHostAddress(m_ip), m_sendPort);
                }
            }else
            {
                qDebug() << "Error while opening file!";
            }
        }
    }
}

