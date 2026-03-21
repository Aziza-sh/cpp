#include <iostream>

int main() {
  int *ptr = new int(10);
  std::cout << "ptr = " << *ptr << std::endl;
  delete ptr;

  int arr[5];
  for (int i = 0; i < 5; ++i) {
    arr[i] = i;
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  int x = 0;
  while (x < 10) {
    std::cout << x << " ";
    ++x;
  }
  std::cout << std::endl;

  return 0;
}