#include <iostream>

int main() {
    const int SIZE = 10;
    int arr[SIZE] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    int target;

    std::cout << "Массив: ";
    for (int i = 0; i < SIZE; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\nВведите число для поиска: ";
    std::cin >> target;

    int index = -1;
    for (int i = 0; i < SIZE; ++i) {
        if (arr[i] == target) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        std::cout << "Элемент найден на позиции " << index << "\n";
    } else {
        std::cout << "Элемент не найден\n";
    }

    return 0;
}