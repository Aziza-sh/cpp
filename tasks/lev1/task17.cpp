#include <iostream>

void swapByValue(int a, int b) {
  int temp = a;
  a = b;
  b = temp;
}

void swapByReference(int &a, int &b) {
  int temp = a;
  a = b;
  b = temp;
}

void swapByPointer(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

int main() {
  int x = 5, y = 10;

  std::cout << "Исходные: x = " << x << ", y = " << y << "\n";

  swapByValue(x, y);
  std::cout << "После swapByValue: x = " << x << ", y = " << y
            << " (без изменений)\n";

  swapByReference(x, y);
  std::cout << "После swapByReference: x = " << x << ", y = " << y
            << " (изменились)\n";

  swapByReference(x, y);
  std::cout << "Возврат обратно swapByReference: x = " << x << ", y = " << y
            << "\n";

  swapByPointer(&x, &y);
  std::cout << "После swapByPointer: x = " << x << ", y = " << y
            << " (изменились)\n";

  return 0;
}