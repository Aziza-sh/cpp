#include <fstream>
#include <iostream>

int main() {
  const int SIZE = 10;
  int arr[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  std::ofstream out("data.bin", std::ios::binary);
  if (!out) {
    std::cerr << "Не удалось создать файл\n";
    return 1;
  }
  out.write(reinterpret_cast<char *>(arr), sizeof(arr));
  out.close();
  std::cout << "Массив записан в бинарный файл.\n";

  int readArr[SIZE];
  std::ifstream in("data.bin", std::ios::binary);
  if (!in) {
    std::cerr << "Не удалось открыть файл\n";
    return 1;
  }
  in.read(reinterpret_cast<char *>(readArr), sizeof(readArr));
  in.close();

  std::cout << "Прочитанный массив: ";
  for (int i = 0; i < SIZE; ++i) {
    std::cout << readArr[i] << " ";
  }
  std::cout << "\n";

  return 0;
}