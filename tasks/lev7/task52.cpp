#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

void bubbleSort(std::vector<int> &arr) {
  int n = arr.size();
  for (int i = 0; i < n - 1; ++i) {
    for (int j = 0; j < n - i - 1; ++j) {
      if (arr[j] > arr[j + 1]) {
        std::swap(arr[j], arr[j + 1]);
      }
    }
  }
}

void insertionSort(std::vector<int> &arr) {
  int n = arr.size();
  for (int i = 1; i < n; ++i) {
    int key = arr[i];
    int j = i - 1;
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      --j;
    }
    arr[j + 1] = key;
  }
}

void quickSort(std::vector<int> &arr, int left, int right) {
  if (left >= right)
    return;
  int pivot = arr[(left + right) / 2];
  int i = left, j = right;
  while (i <= j) {
    while (arr[i] < pivot)
      ++i;
    while (arr[j] > pivot)
      --j;
    if (i <= j) {
      std::swap(arr[i], arr[j]);
      ++i;
      --j;
    }
  }
  quickSort(arr, left, j);
  quickSort(arr, i, right);
}

template <typename Func>
double measureTime(Func sortFunc, std::vector<int> data) {
  auto start = std::chrono::high_resolution_clock::now();
  sortFunc(data);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  return elapsed.count();
}

int main() {
  const int SIZE = 10000;
  std::srand(std::time(nullptr));
  std::vector<int> original(SIZE);
  for (int i = 0; i < SIZE; ++i) {
    original[i] = std::rand() % 10000;
  }

  std::cout << "Размер массива: " << SIZE << "\n";

  std::vector<int> data = original;
  double bubble = measureTime(bubbleSort, data);
  std::cout << "Пузырьковая сортировка: " << bubble << " сек\n";

  data = original;
  double insertion = measureTime(insertionSort, data);
  std::cout << "Сортировка вставками: " << insertion << " сек\n";

  data = original;
  auto quick = [](std::vector<int> &arr) { quickSort(arr, 0, arr.size() - 1); };
  double quickTime = measureTime(quick, data);
  std::cout << "Быстрая сортировка: " << quickTime << " сек\n";

  data = original;
  double stdSort = measureTime(
      [](std::vector<int> &arr) { std::sort(arr.begin(), arr.end()); }, data);
  std::cout << "std::sort: " << stdSort << " сек\n";

  return 0;
}