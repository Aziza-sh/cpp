#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Student {
  std::string firstName;
  std::string lastName;
  int grades[3];

  double average() const { return (grades[0] + grades[1] + grades[2]) / 3.0; }

  bool hasFail() const {
    return grades[0] == 2 || grades[1] == 2 || grades[2] == 2;
  }
};

int main() {
  const int N = 10;
  Student students[N] = {
      {"Иван", "Иванов", {5, 4, 5}},   {"Петр", "Петров", {3, 2, 4}},
      {"Сидор", "Сидоров", {2, 2, 3}}, {"Анна", "Аннова", {5, 5, 5}},
      {"Мария", "Мариева", {4, 4, 4}}, {"Алексей", "Алексеев", {3, 3, 3}},
      {"Елена", "Еленова", {5, 4, 4}}, {"Дмитрий", "Дмитриев", {2, 3, 3}},
      {"Ольга", "Ольгова", {4, 5, 5}}, {"Николай", "Николаев", {3, 4, 3}}};

  int bestIndex = 0;
  for (int i = 1; i < N; ++i) {
    if (students[i].average() > students[bestIndex].average()) {
      bestIndex = i;
    }
  }
  std::cout << "Студент с максимальным средним баллом: "
            << students[bestIndex].firstName << " "
            << students[bestIndex].lastName << " (средний балл "
            << students[bestIndex].average() << ")\n";

  std::cout << "\nСтуденты с двойками:\n";
  for (const auto &s : students) {
    if (s.hasFail()) {
      std::cout << s.firstName << " " << s.lastName
                << " (оценки: " << s.grades[0] << " " << s.grades[1] << " "
                << s.grades[2] << ")\n";
    }
  }

  std::sort(std::begin(students), std::end(students),
            [](const Student &a, const Student &b) {
              return a.lastName < b.lastName;
            });

  std::cout << "\nСтуденты, отсортированные по фамилии:\n";
  for (const auto &s : students) {
    std::cout << s.lastName << " " << s.firstName << "\n";
  }

  return 0;
}