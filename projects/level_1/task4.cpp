#include <codecvt>
#include <cwctype>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <sstream>
#include <string>

// Очистка слова от знаков препинания и приведение к нижнему регистру
std::string cleanWord(const std::string &raw) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  std::wstring wide = converter.from_bytes(raw);

  std::wstring result;
  for (wchar_t ch : wide) {
    if (std::iswalnum(ch)) { // оставляем только буквы и цифры
      result += std::towlower(ch); // нижний регистр с учётом локали
    }
  }
  return converter.to_bytes(result);
}

int main() {
  // Пытаемся установить системную локаль (должна поддерживать русский язык)
  try {
    std::locale::global(std::locale(""));
  } catch (const std::runtime_error &e) {
    std::cerr << "Предупреждение: не удалось установить локаль. "
              << "Русские символы могут обрабатываться неправильно.\n";
  }

  std::string filename;
  std::cout << "Имя файла: ";
  std::cin >> filename;

  std::ifstream f(filename);
  if (!f) {
    std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
    return 1;
  }

  int lines = 0, words = 0, chars = 0;
  std::map<std::string, int> freq;
  std::string longestWord;

  std::string line;
  while (std::getline(f, line)) {
    lines++;
    chars += line.length() + 1; // +1 за символ перевода строки

    std::istringstream iss(line);
    std::string rawWord;
    while (iss >> rawWord) {
      std::string word = cleanWord(rawWord);
      if (!word.empty()) {
        words++;
        freq[word]++;
        if (word.length() > longestWord.length()) {
          longestWord = word;
        }
      }
    }
  }

  if (f.bad()) {
    std::cerr << "Ошибка при чтении файла\n";
    return 1;
  }

  std::cout << "Строк: " << lines << "\nСлов: " << words
            << "\nСимволов: " << chars << std::endl;

  std::string search;
  std::cout << "Слово для подсчёта частоты: ";
  std::cin >> search;
  search = cleanWord(search);
  std::cout << "Частота: " << freq[search] << std::endl;

  if (!freq.empty()) {
    std::string mostFreq;
    int maxFreq = 0;
    for (const auto &p : freq) {
      if (p.second > maxFreq) {
        maxFreq = p.second;
        mostFreq = p.first;
      }
    }
    std::cout << "Самое частое слово: " << mostFreq << " (" << maxFreq << ")\n";
  } else {
    std::cout << "Нет слов в файле\n";
  }

  std::cout << "Самое длинное слово: " << longestWord << std::endl;
  return 0;
}