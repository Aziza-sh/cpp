#include <iostream>
#include <vector>

using namespace std;

void quickSort(vector<int> &arr, int left, int right) {
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
      swap(arr[i], arr[j]);
      ++i;
      --j;
    }
  }
  quickSort(arr, left, j);
  quickSort(arr, i, right);
}

void merge(vector<int> &arr, int left, int mid, int right) {
  int n1 = mid - left + 1;
  int n2 = right - mid;
  vector<int> L(n1), R(n2);
  for (int i = 0; i < n1; ++i)
    L[i] = arr[left + i];
  for (int i = 0; i < n2; ++i)
    R[i] = arr[mid + 1 + i];
  int i = 0, j = 0, k = left;
  while (i < n1 && j < n2) {
    if (L[i] <= R[j])
      arr[k++] = L[i++];
    else
      arr[k++] = R[j++];
  }
  while (i < n1)
    arr[k++] = L[i++];
  while (j < n2)
    arr[k++] = R[j++];
}

void mergeSort(vector<int> &arr, int left, int right) {
  if (left >= right)
    return;
  int mid = left + (right - left) / 2;
  mergeSort(arr, left, mid);
  mergeSort(arr, mid + 1, right);
  merge(arr, left, mid, right);
}

int main() {
  vector<int> arr1 = {3, 6, 8, 10, 1, 2, 1};
  vector<int> arr2 = arr1;

  cout << "Исходный массив: ";
  for (int x : arr1)
    cout << x << " ";
  cout << "\n";

  quickSort(arr1, 0, arr1.size() - 1);
  cout << "Быстрая сортировка: ";
  for (int x : arr1)
    cout << x << " ";
  cout << "\n";

  mergeSort(arr2, 0, arr2.size() - 1);
  cout << "Сортировка слиянием: ";
  for (int x : arr2)
    cout << x << " ";
  cout << "\n";

  return 0;
}