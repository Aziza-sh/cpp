#include <QTcpServer>
#include <QTcpSocket>
#include <QCoreApplication>
#include <QList>
#include <QTextStream>
#include <QThread>
#include <QDebug>

class ChatServer : public QTcpServer {
    Q_OBJECT

public:
    ChatServer(QObject *parent = nullptr) : QTcpServer(parent) {
        if (listen(QHostAddress::Any, 1234)) {
            qDebug() << "Сервер запущен на порту 1234";
        } else {
            qDebug() << "Ошибка запуска сервера";
        }
    }

protected:
    void incomingConnection(qintptr socketDescriptor) override {
        QTcpSocket *client = new QTcpSocket(this);
        client->setSocketDescriptor(socketDescriptor);
        clients.append(client);
        connect(client, &QTcpSocket::readyRead, this, &ChatServer::readMessage);
        connect(client, &QTcpSocket::disconnected, this, &ChatServer::clientDisconnected);
        qDebug() << "Клиент подключен";
    }

private slots:
    void readMessage() {
        QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
        if (!client) return;
        QByteArray data = client->readAll();
        QString message = QString::fromUtf8(data).trimmed();
        if (message.isEmpty()) return;
        qDebug() << "Получено:" << message;
        broadcastMessage(message, client);
    }

    void clientDisconnected() {
        QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
        if (client) {
            clients.removeAll(client);
            client->deleteLater();
            qDebug() << "Клиент отключен";
        }
    }

private:
    void broadcastMessage(const QString &message, QTcpSocket *sender) {
        QByteArray msg = message.toUtf8();
        for (QTcpSocket *client : clients) {
            if (client != sender) {
                client->write(msg + "\n");
            }
        }
    }

    QList<QTcpSocket*> clients;
};

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    ChatServer server;
    return a.exec();
}

#include "task45_serv.moc"