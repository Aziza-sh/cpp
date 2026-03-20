#include <iostream>
#include <string>

int main() {
    std::string password;
    const std::string correct = "secret";

    do {
        std::cout << "Введите пароль: ";
        std::cin >> password;
    } while (password != correct);

    std::cout << "Доступ разрешён!\n";
    return 0;
}