#include <fstream>
#include <iomanip>
#include <iostream>

int main() {
  std::ofstream file("table.txt");
  if (!file) {
    std::cerr << "Не удалось создать файл\n";
    return 1;
  }

  file << std::left << std::setw(10) << "x" << std::setw(10) << "y = x^2"
       << "\n";
  file << "--------------------\n";

  for (int x = -5; x <= 5; ++x) {
    file << std::left << std::setw(10) << x << std::setw(10) << x * x << "\n";
  }

  file.close();
  std::cout << "Таблица сохранена в table.txt\n";
  return 0;
}