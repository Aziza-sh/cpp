#include <iostream>
#include <string>

class Account {
private:
  std::string number;
  double balance;

public:
  Account(const std::string &num, double initialBalance = 0.0)
      : number(num), balance(initialBalance) {}

  void deposit(double amount) {
    if (amount > 0)
      balance += amount;
  }

  bool withdraw(double amount) {
    if (amount > 0 && amount <= balance) {
      balance -= amount;
      return true;
    }
    return false;
  }

  double getBalance() const { return balance; }

  std::string getNumber() const { return number; }
};

int main() {
  Account acc("123456", 1000.0);
  std::cout << "Счет " << acc.getNumber() << ", баланс: " << acc.getBalance()
            << std::endl;
  acc.deposit(500);
  std::cout << "После пополнения: " << acc.getBalance() << std::endl;
  acc.withdraw(200);
  std::cout << "После снятия: " << acc.getBalance() << std::endl;
  return 0;
}