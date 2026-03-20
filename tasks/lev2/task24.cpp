#include <iostream>

int main() {
  for (int i = 0; i < 100; ++i) {
    int *leak = new int[1000];
    if (i % 20 == 0)
      std::cout << "Итерация " << i << ": утечка "
                << (i + 1) * 1000 * sizeof(int) << " байт\n";
  }
  std::cout << "Программа завершена. Память не была освобождена.\n";
  return 0;
}