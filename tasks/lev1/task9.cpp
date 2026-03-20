#include <iostream>
#include <algorithm>

int main() {
    const int SIZE = 10;
    int arr[SIZE] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int target;

    std::cout << "Отсортированный массив: ";
    for (int i = 0; i < SIZE; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\nВведите число для поиска: ";
    std::cin >> target;

    int left = 0, right = SIZE - 1;
    int index = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            index = mid;
            break;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    if (index != -1) {
        std::cout << "Элемент найден на позиции " << index << "\n";
    } else {
        std::cout << "Элемент не найден\n";
    }

    return 0;
}