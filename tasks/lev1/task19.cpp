#include <iostream>

int globalVar = 100;

void demonstrateStatic() {
  static int staticVar = 0;
  int localVar = 0;

  ++globalVar;
  ++staticVar;
  ++localVar;

  std::cout << "globalVar = " << globalVar << ", staticVar = " << staticVar
            << ", localVar = " << localVar << "\n";
}

int main() {
  std::cout << "В начале main: globalVar = " << globalVar << "\n";

  for (int i = 0; i < 3; ++i) {
    demonstrateStatic();
  }

  std::cout << "После вызовов: globalVar = " << globalVar << "\n";

  return 0;
}