#ifndef TASK5_H
#define TASK5_H

#include <QDialog>
#include <QDoubleValidator>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QVector>
#include <ctime>

#pragma pack(push, 1)
struct Account {
  char cardNumber[17];
  char pinHash[17];
  double balance;
  bool active;
  Account() {
    memset(cardNumber, 0, 17);
    memset(pinHash, 0, 17);
    balance = 0.0;
    active = true;
  }
};
#pragma pack(pop)

class Logger {
public:
  static void log(const QString &message);
};

class AccountManager {
public:
  AccountManager(const QString &filename = "accounts.dat");
  bool load();
  int findAccount(const QString &cardNumber) const;
  Account readAccount(int index) const;
  bool writeAccount(int index, const Account &acc);
  bool addAccount(const Account &acc);
  int count() const;
  bool isCardExists(const QString &cardNumber) const;

private:
  QString filename;
  mutable QFile file;
};

class PinCrypto {
public:
  static QString encrypt(const QString &pin);
  static bool verify(const QString &pin, const QString &hash);
};

class PinWindow : public QDialog {
  Q_OBJECT
public:
  PinWindow(QWidget *parent = nullptr);
signals:
  void pinEntered(bool success, double balance, const QString &cardNumber);
private slots:
  void onOk();

private:
  QLineEdit *cardEdit;
  QLineEdit *pinEdit;
};

class MainMenuWindow : public QMainWindow {
  Q_OBJECT
public:
  MainMenuWindow(QWidget *parent = nullptr);
  void setCardNumber(const QString &card) { cardNumber = card; }
  void setBalance(double bal) {
    balance = bal;
    balanceLabel->setText(QString("Баланс: %1 руб.").arg(balance, 0, 'f', 2));
  }
signals:
  void logout();
private slots:
  void onBalance();
  void onWithdraw();
  void onTransfer();
  void onExit();

private:
  QString cardNumber;
  double balance;
  QLabel *balanceLabel;
};

class OperationWindow : public QDialog {
  Q_OBJECT
public:
  enum Mode { Withdraw, Transfer };
  OperationWindow(Mode mode, QWidget *parent = nullptr);
  void setCardNumber(const QString &card) { cardNumber = card; }
  void setBalance(double bal) { balance = bal; }
signals:
  void operationDone(bool success, double newBalance, const QString &message);
private slots:
  void onOk();

private:
  Mode mode;
  QString cardNumber;
  double balance;
  QLineEdit *amountEdit;
  QLineEdit *targetCardEdit;
};

class AtmController : public QObject {
  Q_OBJECT
public:
  AtmController();
private slots:
  void start();
  void onPinEntered(bool success, double balance, const QString &cardNumber);
  void onLogout();

private:
  AccountManager accMgr;
  PinWindow *pinWindow;
  MainMenuWindow *menuWindow;
};

#endif