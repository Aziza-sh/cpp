#include <cstring>
#include <iostream>

class String {
private:
  char *data;
  size_t length;

public:
  String() : data(nullptr), length(0) {}

  String(const char *str) {
    length = strlen(str);
    data = new char[length + 1];
    strcpy(data, str);
    std::cout << "Конструктор: " << data << std::endl;
  }

  String(const String &other) {
    length = other.length;
    data = new char[length + 1];
    strcpy(data, other.data);
    std::cout << "Конструктор копирования: " << data << std::endl;
  }

  String(String &&other) noexcept : data(other.data), length(other.length) {
    other.data = nullptr;
    other.length = 0;
    std::cout << "Конструктор перемещения" << std::endl;
  }

  ~String() {
    std::cout << "Деструктор: " << (data ? data : "nullptr") << std::endl;
    delete[] data;
  }

  String &operator=(const String &other) {
    if (this != &other) {
      delete[] data;
      length = other.length;
      data = new char[length + 1];
      strcpy(data, other.data);
      std::cout << "Оператор присваивания копированием: " << data << std::endl;
    }
    return *this;
  }

  String &operator=(String &&other) noexcept {
    if (this != &other) {
      delete[] data;
      data = other.data;
      length = other.length;
      other.data = nullptr;
      other.length = 0;
      std::cout << "Оператор присваивания перемещением" << std::endl;
    }
    return *this;
  }

  void print() const {
    if (data)
      std::cout << data << std::endl;
    else
      std::cout << "nullptr" << std::endl;
  }
};

int main() {
  std::cout << "Конструктор" << std::endl;
  String s1("Hello");

  std::cout << "\nКонструктор копирования" << std::endl;
  String s2 = s1;

  std::cout << "\nКонструктор перемещения" << std::endl;
  String s3 = std::move(s1);

  std::cout << "\nСостояние s1 после перемещения" << std::endl;
  s1.print();

  std::cout << "\nОператор присваивания копированием" << std::endl;
  String s4;
  s4 = s2;

  std::cout << "\nОператор присваивания перемещением" << std::endl;
  String s5;
  s5 = std::move(s2);

  std::cout << "\nКонец main " << std::endl;
  return 0;
}