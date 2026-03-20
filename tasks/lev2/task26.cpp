#include <iostream>

int main() {
  const int SIZE = 5;
  int arr[SIZE] = {10, 20, 30, 40, 50};

  std::cout << "Демонстрация эквивалентности записи arr[i] и *(arr + i)\n\n";

  for (int i = 0; i < SIZE; ++i) {
    std::cout << "arr[" << i << "] = " << arr[i] << "   *(arr + " << i
              << ") = " << *(arr + i) << "\n";
  }

  int *ptr = arr;
  std::cout << "\nОбход с помощью указателя:\n";
  for (int i = 0; i < SIZE; ++i) {
    std::cout << "*(ptr + " << i << ") = " << *(ptr + i) << "\n";
  }

  return 0;
}