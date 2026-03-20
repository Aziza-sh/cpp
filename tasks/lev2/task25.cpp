#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

int main() {
  int size;
  std::cout << "Введите размер массива: ";
  std::cin >> size;

  int *arr = new int[size];

  std::srand(std::time(nullptr));
  for (int i = 0; i < size; ++i) {
    arr[i] = std::rand() % 100;
  }

  std::cout << "Исходный массив: ";
  for (int i = 0; i < size; ++i) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  std::sort(arr, arr + size);

  std::cout << "Отсортированный массив: ";
  for (int i = 0; i < size; ++i) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  delete[] arr;
  std::cout << "Память освобождена.\n";

  return 0;
}