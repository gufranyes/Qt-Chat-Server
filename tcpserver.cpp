#include "tcpserver.h"

tcpServer::tcpServer(QObject *parent) : QObject(parent)
{
    connect(&_server, &QTcpServer::newConnection, this, &tcpServer::onNewConnection);
    connect(this, &tcpServer::newMessage, this, &tcpServer::onNewMessage);
    if(_server.listen(QHostAddress::Any, 45000)){
        qInfo() << "Listening Succesfully...";
    }
}
void tcpServer::sendMessage(const QString &message){
    emit newMessage("Server: " + message.toUtf8());
}
void tcpServer::onNewConnection()
{
    const auto client = _server.nextPendingConnection();
    if(client == nullptr){return;}

    qInfo() << "New client connected.";

    _clients.insert(this->getClientKey(client), client);

    connect(client, &QTcpSocket::readyRead, this, &tcpServer::onReadyRead);
    connect(client, &QTcpSocket::disconnected, this, &tcpServer::onDisconnected);
}

void tcpServer::onReadyRead()
{
    const auto client = qobject_cast<QTcpSocket*>(sender());
    if(client == nullptr){return;}

    const auto messages = this->getClientKey(client).toUtf8() + ": " + client->readAll();

    emit newMessage(messages);
}

void tcpServer::onDisconnected()
{
    const auto client = qobject_cast<QTcpSocket*>(sender());
    if(client == nullptr){return;}

    _clients.remove(this->getClientKey(client));
}

void tcpServer::onNewMessage(const QByteArray &byteArrayMessage)
{
    for(const auto &client: qAsConst(_clients)){
        client->write(byteArrayMessage);
        client->flush();
    }

}

QString tcpServer::getClientKey(const QTcpSocket *client) const
{
    return client->peerAddress().toString().append(":").append(QString::number(client->peerPort()));
}
