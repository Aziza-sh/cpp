#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    const int SIZE = 10;
    int arr[SIZE];

    std::srand(std::time(nullptr));
    for (int i = 0; i < SIZE; ++i) {
        arr[i] = std::rand() % 201 - 100; // случайные числа от -100 до 100
    }

    std::cout << "Массив: ";
    for (int i = 0; i < SIZE; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    int max = arr[0], min = arr[0];
    double sum = 0;
    int positiveSum = 0;

    for (int i = 0; i < SIZE; ++i) {
        if (arr[i] > max) max = arr[i];
        if (arr[i] < min) min = arr[i];
        sum += arr[i];
        if (arr[i] > 0) positiveSum += arr[i];
    }

    double average = sum / SIZE;

    std::cout << "Максимум: " << max << "\n";
    std::cout << "Минимум: " << min << "\n";
    std::cout << "Среднее арифметическое: " << average << "\n";
    std::cout << "Сумма положительных: " << positiveSum << "\n";

    return 0;
}