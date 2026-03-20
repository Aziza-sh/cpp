#include <iostream>
#include <string>

void printString(const std::string &text, int repeat = 1,
                 char separator = ' ') {
  for (int i = 0; i < repeat; ++i) {
    std::cout << text;
    if (i != repeat - 1)
      std::cout << separator;
  }
  std::cout << "\n";
}

int main() {
  std::cout << "Вызов с одним параметром:\n";
  printString("Hello");

  std::cout << "Вызов с двумя параметрами:\n";
  printString("Hi", 3);

  std::cout << "Вызов с тремя параметрами:\n";
  printString("World", 5, '-');

  return 0;
}