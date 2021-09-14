#include "client.h"

Client::Client(QObject *parent, QString ip, quint32 port, quint32 receivePort) : QObject(parent),
    m_ip(ip),
    m_port(port),
    m_receivePort(receivePort),
    m_bytesReceived(0),
    m_packetsQuantityWaiting(0),
    m_writePacketsCounter(1),
    m_sendSocket(nullptr),
    m_receiveSocket(nullptr)
{
    m_receiveSocket = new QUdpSocket(this);
    if(m_receiveSocket)
    {
        m_receiveSocket->bind(QHostAddress(m_ip), m_receivePort);

        connect(m_receiveSocket, SIGNAL(readyRead()),
                this, SLOT(readPendingDatagrams()));
    }

    m_sendSocket = new QUdpSocket(this);
    if(m_sendSocket)
    {
        m_sendSocket->connectToHost(QHostAddress(ip), port);
    }

    m_parseTimer = new QTimer(this);
    connect(m_parseTimer, &QTimer::timeout, this, &Client::parseBuffer);
    sendStart();
}

Client::~Client()
{
    if(m_receiveSocket)
    {
        delete m_receiveSocket;
    }
    if(m_sendSocket)
    {
        delete m_sendSocket;
    }
}

void Client::sendStart()
{
    QJsonObject textObject;
    textObject["command"] = "start";

    QJsonDocument doc(textObject);
    QByteArray jByte(doc.toJson(QJsonDocument::Compact));
    m_sendSocket->write(jByte);
}

void Client::parseBuffer()
{
    while(!m_dataList.isEmpty())
    {
        for(int i = 0; i < m_dataList.length(); ++i)
        {
            QDataStream stream(m_dataList.at(i));

            quint8 type = 0;
            quint32 size = 0;
            quint32 packetsCounter = 0;
            quint16 readed = 0;

            stream >> type;
            if(type == 0x01)
            {
                stream >> size;//full size - 4 bytes
                stream >> packetsCounter;//packet number - 4 bytes
                stream >> readed;//packet size

                if(packetsCounter == m_writePacketsCounter)
                {
                    QByteArray data(readed, Qt::Uninitialized);

                    stream >> data;

                    m_buffer.append(data);
                    m_writePacketsCounter++;
                    m_bytesReceived += readed;

                    m_dataList.removeAt(i);
                    --i;

                    if(size == m_bytesReceived)
                    {
                        QTemporaryDir dir;
                        QString path = dir.path() + "temp.bmp";
                        QFile file(path);
                        bool res = file.open(QFile::ReadWrite);
                        if(res)
                        {
                            file.write(m_buffer);
                            file.close();
                            emit fileDownloaded(path);
                        }else
                        {
                            qDebug() << "Can not open the file!";
                        }
                    }
                }
            }else
            {
                qDebug() << "Unknown packet type!";
                m_dataList.removeAt(i);
                --i;
            }
        }
    }
}

void Client::readPendingDatagrams()
{
    while (m_receiveSocket->hasPendingDatagrams())
    {
        QByteArray datagram;        
        datagram.resize(m_receiveSocket->pendingDatagramSize());
        m_receiveSocket->readDatagram(datagram.data(), datagram.size());

        if(m_dataList.isEmpty())
        {
            QDataStream stream(datagram);
            quint8 type = 0;
            quint32 size = 0;
            stream >> type;
            stream >> size;//full size - 4 bytes
            m_packetsQuantityWaiting = size/200;            
        }
        m_dataList.append(datagram);

        if(m_dataList.length() == static_cast<int>(m_packetsQuantityWaiting))
        {
            m_parseTimer->start(1);
        }
    }
    emit dataTransfer();
}
