// 11_structure.cpp
#include <iostream>
#include <string>

struct Student {
  std::string firstName;
  std::string lastName;
  int grades[3];
};

int main() {
  Student s;
  std::cout << "Введите имя: ";
  std::cin >> s.firstName;
  std::cout << "Введите фамилию: ";
  std::cin >> s.lastName;
  std::cout << "Введите 3 оценки: ";
  std::cin >> s.grades[0] >> s.grades[1] >> s.grades[2];

  std::cout << "\nСтудент: " << s.firstName << " " << s.lastName << "\n";
  std::cout << "Оценки: " << s.grades[0] << " " << s.grades[1] << " "
            << s.grades[2] << "\n";

  return 0;
}