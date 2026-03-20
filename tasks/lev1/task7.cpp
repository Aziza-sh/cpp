#include <iostream>

int main() {
    const int SIZE = 10;
    int arr[SIZE] = {0, 1, 0, 3, 0, 0, 6, 7, 0, 9};
    int newSize = SIZE;

    std::cout << "Исходный массив: ";
    for (int i = 0; i < SIZE; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    for (int i = 0; i < newSize; ) {
        if (arr[i] == 0) {
            for (int j = i; j < newSize - 1; ++j) {
                arr[j] = arr[j + 1];
            }
            --newSize;
        } else {
            ++i;
        }
    }

    std::cout << "После удаления нулей: ";
    for (int i = 0; i < newSize; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    return 0;
}