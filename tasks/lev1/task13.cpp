#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int countWords(const std::string &filename) {
  std::ifstream file(filename);
  if (!file) {
    std::cerr << "Не удалось открыть файл " << filename << "\n";
    return -1;
  }
  int count = 0;
  std::string word;
  while (file >> word) {
    ++count;
  }
  return count;
}

void copyFile(const std::string &src, const std::string &dst) {
  std::ifstream in(src);
  std::ofstream out(dst);
  if (!in || !out) {
    std::cerr << "Ошибка открытия файлов\n";
    return;
  }
  out << in.rdbuf();
  std::cout << "Файл скопирован.\n";
}

void removeEmptyLines(const std::string &filename) {
  std::ifstream in(filename);
  if (!in) {
    std::cerr << "Не удалось открыть файл\n";
    return;
  }
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(in, line)) {
    if (!line.empty()) {
      lines.push_back(line);
    }
  }
  in.close();

  std::ofstream out(filename);
  if (!out) {
    std::cerr << "Не удалось открыть файл для записи\n";
    return;
  }
  for (const auto &l : lines) {
    out << l << "\n";
  }
  std::cout << "Пустые строки удалены.\n";
}

int main() {
  std::ofstream test("input.txt");
  test << "Hello world\n\nThis is a test\n\n\nEnd of file.\n";
  test.close();

  std::cout << "Количество слов в input.txt: " << countWords("input.txt")
            << "\n";
  copyFile("input.txt", "copy.txt");
  removeEmptyLines("input.txt");

  return 0;
}