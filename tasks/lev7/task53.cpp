#include <iostream>
#include <limits>
#include <string>

int getInt(const std::string &prompt, int minVal, int maxVal) {
  int value;
  while (true) {
    std::cout << prompt;
    std::cin >> value;

    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Ошибка: введите целое число.\n";
    } else if (value < minVal || value > maxVal) {
      std::cout << "Ошибка: число должно быть от " << minVal << " до " << maxVal
                << ".\n";
    } else {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return value;
    }
  }
}

std::string getString(const std::string &prompt, bool allowEmpty = false) {
  std::string line;
  while (true) {
    std::cout << prompt;
    std::getline(std::cin, line);
    if (!allowEmpty && line.empty()) {
      std::cout << "Ошибка: строка не может быть пустой.\n";
    } else {
      return line;
    }
  }
}

int main() {
  std::cout << "Валидация ввода \n";
  int age = getInt("Введите ваш возраст (0-120): ", 0, 120);
  std::string name = getString("Введите ваше имя: ", false);
  double weight;
  while (true) {
    std::cout << "Введите ваш вес (кг): ";
    if (std::cin >> weight && weight > 0 && weight < 500) {
      break;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Ошибка: введите положительное число.\n";
  }

  std::cout << "\nИнформация:\n";
  std::cout << "Имя: " << name << "\n";
  std::cout << "Возраст: " << age << "\n";
  std::cout << "Вес: " << weight << " кг\n";

  return 0;
}