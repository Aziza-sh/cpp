#include <iostream>
#include <string>

int main() {
    std::string name;
    int birthYear, currentYear = 2026; 

    std::cout << "Введите ваше имя: ";
    std::getline(std::cin, name);

    std::cout << "Введите ваш год рождения: ";
    std::cin >> birthYear;

    int age = currentYear - birthYear;
    std::cout << "Привет, " << name << "! Ваш возраст: " << age << " лет.\n";

    return 0;
}