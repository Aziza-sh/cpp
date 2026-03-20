#include <iostream>

int binarySearchRecursive(const int arr[], int left, int right, int target) {
  if (left > right)
    return -1;

  int mid = left + (right - left) / 2;
  if (arr[mid] == target)
    return mid;
  if (arr[mid] < target)
    return binarySearchRecursive(arr, mid + 1, right, target);
  else
    return binarySearchRecursive(arr, left, mid - 1, target);
}

int main() {
  const int SIZE = 10;
  int arr[SIZE] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
  int target = 14;

  int index = binarySearchRecursive(arr, 0, SIZE - 1, target);

  if (index != -1) {
    std::cout << "Элемент " << target << " найден на позиции " << index << "\n";
  } else {
    std::cout << "Элемент не найден\n";
  }

  return 0;
}