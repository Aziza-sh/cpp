#include <QApplication>
#include <QWidget>
#include <QTcpSocket>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

class ChatClient : public QWidget {
    Q_OBJECT

public:
    ChatClient() {
        setWindowTitle("Чат клиент");
        resize(500, 400);

        QVBoxLayout *layout = new QVBoxLayout(this);
        textDisplay = new QTextEdit(this);
        textDisplay->setReadOnly(true);
        layout->addWidget(textDisplay);

        QHBoxLayout *inputLayout = new QHBoxLayout;
        lineEdit = new QLineEdit(this);
        QPushButton *sendBtn = new QPushButton("Отправить", this);
        inputLayout->addWidget(lineEdit);
        inputLayout->addWidget(sendBtn);
        layout->addLayout(inputLayout);

        connect(sendBtn, &QPushButton::clicked, this, &ChatClient::sendMessage);
        connect(lineEdit, &QLineEdit::returnPressed, this, &ChatClient::sendMessage);

        socket = new QTcpSocket(this);
        connect(socket, &QTcpSocket::connected, this, &ChatClient::connected);
        connect(socket, &QTcpSocket::readyRead, this, &ChatClient::readMessage);
        connect(socket, &QTcpSocket::errorOccurred, this, &ChatClient::displayError);

        socket->connectToHost("127.0.0.1", 1234);
    }

private slots:
    void connected() {
        textDisplay->append("Подключено к серверу");
    }

    void readMessage() {
        QByteArray data = socket->readAll();
        QString message = QString::fromUtf8(data).trimmed();
        if (!message.isEmpty()) textDisplay->append("Сервер: " + message);
    }

    void sendMessage() {
        QString msg = lineEdit->text().trimmed();
        if (msg.isEmpty()) return;
        socket->write(msg.toUtf8() + "\n");
        textDisplay->append("Я: " + msg);
        lineEdit->clear();
    }

    void displayError(QAbstractSocket::SocketError) {
        QMessageBox::critical(this, "Ошибка", socket->errorString());
    }

private:
    QTcpSocket *socket;
    QTextEdit *textDisplay;
    QLineEdit *lineEdit;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ChatClient client;
    client.show();
    return app.exec();
}

#include "task45_cli.moc"