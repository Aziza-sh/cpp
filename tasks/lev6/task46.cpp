#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

void xorEncryptDecrypt(const std::string &inputFile, const std::string &outputFile, char key) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in) {
        std::cerr << "Не удалось открыть входной файл\n";
        return;
    }
    std::ofstream out(outputFile, std::ios::binary);
    if (!out) {
        std::cerr << "Не удалось создать выходной файл\n";
        return;
    }

    char ch;
    while (in.get(ch)) {
        ch ^= key;
        out.put(ch);
    }
    std::cout << "Операция завершена. Файл сохранён как " << outputFile << "\n";
}

int main() {
    std::string input, output;
    char key;
    int choice;

    std::cout << "1. Зашифровать\n2. Расшифровать (то же самое)\nВыберите действие: ";
    std::cin >> choice;
    std::cin.ignore();

    std::cout << "Введите имя входного файла: ";
    std::getline(std::cin, input);
    std::cout << "Введите имя выходного файла: ";
    std::getline(std::cin, output);
    std::cout << "Введите ключ (один символ): ";
    std::cin >> key;

    xorEncryptDecrypt(input, output, key);
    return 0;
}