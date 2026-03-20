#include <iostream>
#include <string>

struct Student {
  std::string firstName;
  std::string lastName;
  int grades[3];
};

int main() {
  Student *student = new Student;

  std::cout << "Введите имя: ";
  std::cin >> student->firstName;
  std::cout << "Введите фамилию: ";
  std::cin >> student->lastName;
  std::cout << "Введите 3 оценки: ";
  std::cin >> student->grades[0] >> student->grades[1] >> student->grades[2];

  std::cout << "\nСтудент: " << student->firstName << " " << student->lastName
            << "\n";
  std::cout << "Оценки: " << student->grades[0] << " " << student->grades[1]
            << " " << student->grades[2] << "\n";

  delete student;
  std::cout << "Память освобождена.\n";

  return 0;
}