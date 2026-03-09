#include "task5.h"
#include <QApplication>
#include <QDataStream>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

void Logger::log(const QString &message) {
  QFile file("atm.log");
  if (file.open(QIODevice::Append | QIODevice::Text)) {
    QTextStream out(&file);
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " - "
        << message << "\n";
    file.close();
  }
}

AccountManager::AccountManager(const QString &fname)
    : filename(fname), file(fname) {}

bool AccountManager::load() {
  if (!file.exists()) {
    QFile f(filename);
    return f.open(QIODevice::WriteOnly);
  }
  return true;
}

int AccountManager::findAccount(const QString &cardNumber) const {
  if (!file.open(QIODevice::ReadOnly))
    return -1;
  QByteArray cardBytes = cardNumber.toLatin1();
  Account acc;
  int index = 0;
  while (file.read((char *)&acc, sizeof(Account)) == sizeof(Account)) {
    if (strcmp(acc.cardNumber, cardBytes.constData()) == 0) {
      file.close();
      return index;
    }
    index++;
  }
  file.close();
  return -1;
}

Account AccountManager::readAccount(int index) const {
  Account acc;
  if (!file.open(QIODevice::ReadOnly))
    return acc;
  file.seek(index * sizeof(Account));
  file.read((char *)&acc, sizeof(Account));
  file.close();
  return acc;
}

bool AccountManager::writeAccount(int index, const Account &acc) {
  if (!file.open(QIODevice::ReadWrite))
    return false;
  file.seek(index * sizeof(Account));
  qint64 written = file.write((const char *)&acc, sizeof(Account));
  file.close();
  return written == sizeof(Account);
}

bool AccountManager::addAccount(const Account &acc) {
  if (!file.open(QIODevice::Append | QIODevice::WriteOnly))
    return false;
  qint64 written = file.write((const char *)&acc, sizeof(Account));
  file.close();
  return written == sizeof(Account);
}

int AccountManager::count() const {
  if (!file.open(QIODevice::ReadOnly))
    return 0;
  qint64 size = file.size();
  file.close();
  return size / sizeof(Account);
}

bool AccountManager::isCardExists(const QString &cardNumber) const {
  return findAccount(cardNumber) != -1;
}

QString PinCrypto::encrypt(const QString &pin) {
  QByteArray bytes = pin.toLatin1();
  for (int i = 0; i < bytes.size(); ++i) {
    bytes[i] = bytes[i] ^ 0xAA;
  }
  return QString::fromLatin1(bytes.toHex());
}

bool PinCrypto::verify(const QString &pin, const QString &hash) {
  return encrypt(pin) == hash;
}

PinWindow::PinWindow(QWidget *parent) : QDialog(parent) {
  setWindowTitle("Вход в банкомат");
  setFixedSize(350, 200);
  setStyleSheet("QDialog { background-color: #FFF5E6; }"
                "QLineEdit { border: 1px solid #FF8C00; border-radius: 3px; "
                "padding: 5px; }"
                "QPushButton { background-color: #FF8C00; color: white; "
                "border: none; padding: 8px; border-radius: 3px; }"
                "QPushButton:hover { background-color: #FFA500; }"
                "QLabel { color: #8B4513; }");

  QVBoxLayout *layout = new QVBoxLayout(this);

  layout->addWidget(new QLabel("Номер карты:"));
  cardEdit = new QLineEdit;
  cardEdit->setInputMask("9999 9999 9999 9999");
  layout->addWidget(cardEdit);

  layout->addWidget(new QLabel("PIN-код:"));
  pinEdit = new QLineEdit;
  pinEdit->setEchoMode(QLineEdit::Password);
  pinEdit->setMaxLength(4);
  layout->addWidget(pinEdit);

  QPushButton *okButton = new QPushButton("OK");
  layout->addWidget(okButton);
  connect(okButton, &QPushButton::clicked, this, &PinWindow::onOk);
}

void PinWindow::onOk() {
  QString card = cardEdit->text().remove(' ');
  QString pin = pinEdit->text();

  if (card.length() != 16 || !card.toLongLong()) {
    QMessageBox::warning(this, "Ошибка",
                         "Номер карты должен состоять из 16 цифр.");
    return;
  }
  if (pin.length() != 4 || !pin.toInt()) {
    QMessageBox::warning(this, "Ошибка", "PIN должен состоять из 4 цифр.");
    return;
  }

  AccountManager mgr;
  int idx = mgr.findAccount(card);
  if (idx < 0) {
    QMessageBox::critical(this, "Ошибка", "Карта не найдена.");
    Logger::log("Неудачная попытка входа: карта " + card + " не найдена.");
    reject();
    return;
  }

  Account acc = mgr.readAccount(idx);
  if (!acc.active) {
    QMessageBox::critical(this, "Ошибка", "Карта заблокирована.");
    Logger::log("Попытка входа по заблокированной карте " + card);
    reject();
    return;
  }

  QString storedHash = QString::fromLatin1(acc.pinHash);
  if (!PinCrypto::verify(pin, storedHash)) {
    QMessageBox::critical(this, "Ошибка", "Неверный PIN.");
    Logger::log("Неверный PIN для карты " + card);
    reject();
    return;
  }

  Logger::log("Успешный вход: карта " + card);
  emit pinEntered(true, acc.balance, card);
  accept();
}

MainMenuWindow::MainMenuWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Банкомат");
  setFixedSize(400, 300);
  setStyleSheet(
      "QMainWindow { background-color: #FFF5E6; }"
      "QPushButton { background-color: #FF8C00; color: white; border: none; "
      "padding: 8px; border-radius: 3px; font-size: 14px; }"
      "QPushButton:hover { background-color: #FFA500; }"
      "QLabel { color: #8B4513; font-size: 14px; }");

  QWidget *central = new QWidget;
  setCentralWidget(central);
  QVBoxLayout *layout = new QVBoxLayout(central);

  balanceLabel = new QLabel("Баланс: 0.00 руб.");
  layout->addWidget(balanceLabel);

  QPushButton *btnBalance = new QPushButton("Показать баланс");
  QPushButton *btnWithdraw = new QPushButton("Снять наличные");
  QPushButton *btnTransfer = new QPushButton("Перевести средства");
  QPushButton *btnExit = new QPushButton("Выйти");

  layout->addWidget(btnBalance);
  layout->addWidget(btnWithdraw);
  layout->addWidget(btnTransfer);
  layout->addWidget(btnExit);

  connect(btnBalance, &QPushButton::clicked, this, &MainMenuWindow::onBalance);
  connect(btnWithdraw, &QPushButton::clicked, this,
          &MainMenuWindow::onWithdraw);
  connect(btnTransfer, &QPushButton::clicked, this,
          &MainMenuWindow::onTransfer);
  connect(btnExit, &QPushButton::clicked, this, &MainMenuWindow::onExit);
}

void MainMenuWindow::onBalance() {
  QMessageBox::information(
      this, "Баланс", QString("Ваш баланс: %1 руб.").arg(balance, 0, 'f', 2));
  Logger::log("Запрос баланса по карте " + cardNumber);
}

void MainMenuWindow::onWithdraw() {
  OperationWindow win(OperationWindow::Withdraw, this);
  win.setCardNumber(cardNumber);
  win.setBalance(balance);
  connect(&win, &OperationWindow::operationDone, this,
          [this](bool success, double newBalance, const QString &msg) {
            if (success) {
              balance = newBalance;
              balanceLabel->setText(
                  QString("Баланс: %1 руб.").arg(balance, 0, 'f', 2));
              QMessageBox::information(this, "Успех", msg);
            } else {
              QMessageBox::warning(this, "Ошибка", msg);
            }
          });
  win.exec();
}

void MainMenuWindow::onTransfer() {
  OperationWindow win(OperationWindow::Transfer, this);
  win.setCardNumber(cardNumber);
  win.setBalance(balance);
  connect(&win, &OperationWindow::operationDone, this,
          [this](bool success, double newBalance, const QString &msg) {
            if (success) {
              balance = newBalance;
              balanceLabel->setText(
                  QString("Баланс: %1 руб.").arg(balance, 0, 'f', 2));
              QMessageBox::information(this, "Успех", msg);
            } else {
              QMessageBox::warning(this, "Ошибка", msg);
            }
          });
  win.exec();
}

void MainMenuWindow::onExit() {
  Logger::log("Выход из карты " + cardNumber);
  emit logout();
  close();
}

OperationWindow::OperationWindow(Mode m, QWidget *parent)
    : QDialog(parent), mode(m) {
  setWindowTitle(mode == Withdraw ? "Снятие" : "Перевод");
  setFixedSize(350, 200);
  setStyleSheet("QDialog { background-color: #FFF5E6; }"
                "QLineEdit { border: 1px solid #FF8C00; border-radius: 3px; "
                "padding: 5px; }"
                "QPushButton { background-color: #FF8C00; color: white; "
                "border: none; padding: 8px; border-radius: 3px; }"
                "QPushButton:hover { background-color: #FFA500; }"
                "QLabel { color: #8B4513; }");

  QVBoxLayout *layout = new QVBoxLayout(this);
  if (mode == Transfer) {
    layout->addWidget(new QLabel("Номер карты получателя:"));
    targetCardEdit = new QLineEdit;
    targetCardEdit->setInputMask("9999 9999 9999 9999");
    layout->addWidget(targetCardEdit);
  }

  layout->addWidget(new QLabel("Сумма:"));
  amountEdit = new QLineEdit;
  amountEdit->setValidator(new QDoubleValidator(0.01, 1000000.0, 2, this));
  layout->addWidget(amountEdit);

  QPushButton *okButton = new QPushButton("OK");
  layout->addWidget(okButton);
  connect(okButton, &QPushButton::clicked, this, &OperationWindow::onOk);
}

void OperationWindow::onOk() {
  double amount = amountEdit->text().toDouble();
  if (amount <= 0.01) {
    QMessageBox::warning(this, "Ошибка", "Введите корректную сумму.");
    return;
  }

  AccountManager mgr;
  int srcIdx = mgr.findAccount(cardNumber);
  if (srcIdx < 0) {
    emit operationDone(false, balance, "Карта отправителя не найдена.");
    reject();
    return;
  }

  Account src = mgr.readAccount(srcIdx);

  if (mode == Withdraw) {
    if (src.balance < amount) {
      emit operationDone(false, balance, "Недостаточно средств.");
      reject();
      return;
    }
    src.balance -= amount;
    if (!mgr.writeAccount(srcIdx, src)) {
      emit operationDone(false, balance, "Ошибка записи.");
      reject();
      return;
    }
    Logger::log(
        QString("Снятие %1 руб. с карты %2").arg(amount).arg(cardNumber));
    emit operationDone(true, src.balance, QString("Снято %1 руб.").arg(amount));
    accept();
  } else {
    QString targetCard = targetCardEdit->text().remove(' ');
    if (targetCard.isEmpty()) {
      QMessageBox::warning(this, "Ошибка", "Введите номер карты получателя.");
      return;
    }
    int dstIdx = mgr.findAccount(targetCard);
    if (dstIdx < 0) {
      emit operationDone(false, balance, "Карта получателя не найдена.");
      reject();
      return;
    }
    Account dst = mgr.readAccount(dstIdx);
    if (!dst.active) {
      emit operationDone(false, balance, "Карта получателя заблокирована.");
      reject();
      return;
    }
    if (src.balance < amount) {
      emit operationDone(false, balance, "Недостаточно средств.");
      reject();
      return;
    }
    src.balance -= amount;
    dst.balance += amount;
    if (!mgr.writeAccount(srcIdx, src) || !mgr.writeAccount(dstIdx, dst)) {
      emit operationDone(false, balance, "Ошибка записи.");
      reject();
      return;
    }
    Logger::log(QString("Перевод %1 руб. с карты %2 на карту %3")
                    .arg(amount)
                    .arg(cardNumber)
                    .arg(targetCard));
    emit operationDone(true, src.balance,
                       QString("Переведено %1 руб.").arg(amount));
    accept();
  }
}

AtmController::AtmController() {
  AccountManager mgr;
  if (mgr.count() == 0) {
    QStringList cardNumbers = {"1234567890123456", "1111222233334444",
                               "5555666677778888", "9999000011112222",
                               "4444555566667777"};
    QStringList pins = {"1234", "1111", "2222", "3333", "4444"};
    double initialBalance = 10000.0;
    for (int i = 0; i < cardNumbers.size(); ++i) {
      Account acc;
      strcpy(acc.cardNumber, cardNumbers[i].toLatin1().constData());
      QString pinHash = PinCrypto::encrypt(pins[i]);
      strcpy(acc.pinHash, pinHash.toLatin1().constData());
      acc.balance = initialBalance;
      acc.active = true;
      mgr.addAccount(acc);
      Logger::log("Добавлена тестовая карта " + cardNumbers[i] + " с PIN " +
                  pins[i]);
    }
  }
  start();
}

void AtmController::start() {
  pinWindow = new PinWindow;
  connect(pinWindow, &PinWindow::pinEntered, this,
          &AtmController::onPinEntered);
  pinWindow->show();
}

void AtmController::onPinEntered(bool success, double balance,
                                 const QString &cardNumber) {
  if (success) {
    menuWindow = new MainMenuWindow;
    menuWindow->setCardNumber(cardNumber);
    menuWindow->setBalance(balance);
    connect(menuWindow, &MainMenuWindow::logout, this,
            &AtmController::onLogout);
    menuWindow->show();
    pinWindow->close();
  } else {
    pinWindow->close();
    start();
  }
}

void AtmController::onLogout() {
  menuWindow->close();
  start();
}