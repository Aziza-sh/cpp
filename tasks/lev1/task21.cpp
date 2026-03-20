#include <iostream>

void hanoi(int n, char from, char to, char aux) {
  if (n == 1) {
    std::cout << "Переместить диск 1 с " << from << " на " << to << "\n";
    return;
  }
  hanoi(n - 1, from, aux, to);
  std::cout << "Переместить диск " << n << " с " << from << " на " << to
            << "\n";
  hanoi(n - 1, aux, to, from);
}

int main() {
  int disks = 3;
  std::cout << "Решение для " << disks << " дисков:\n";
  hanoi(disks, 'A', 'C', 'B');
  return 0;
}