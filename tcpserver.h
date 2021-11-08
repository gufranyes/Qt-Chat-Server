#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>

class tcpServer : public QObject
{
    Q_OBJECT
public:
    explicit tcpServer(QObject *parent = nullptr);

signals:
    void newMessage(const QByteArray &message);

public slots:
    void sendMessage(const QString &message);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
    void onNewMessage(const QByteArray &byteArrayMessage);

private:

    QTcpServer _server;
    QHash<QString, QTcpSocket*> _clients;

    QString getClientKey(const QTcpSocket *client) const;

};

#endif // TCPSERVER_H
