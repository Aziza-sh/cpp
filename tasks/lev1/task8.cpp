#include <iostream>
#include <iomanip>

int main() {
    const int ROWS = 3;
    const int COLS = 3;
    int matrix[ROWS][COLS] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    std::cout << "Матрица:\n";
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            std::cout << std::setw(4) << matrix[i][j];
        }
        std::cout << "\n";
    }

    std::cout << "Главная диагональ: ";
    for (int i = 0; i < ROWS; ++i) {
        std::cout << matrix[i][i] << " ";
    }
    std::cout << "\n";

    std::cout << "Побочная диагональ: ";
    for (int i = 0; i < ROWS; ++i) {
        std::cout << matrix[i][COLS - 1 - i] << " ";
    }
    std::cout << "\n";

    std::cout << "Суммы по столбцам: ";
    for (int j = 0; j < COLS; ++j) {
        int sum = 0;
        for (int i = 0; i < ROWS; ++i) {
            sum += matrix[i][j];
        }
        std::cout << sum << " ";
    }
    std::cout << "\n";

    int transposed[COLS][ROWS];
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }

    std::cout << "Транспонированная матрица:\n";
    for (int i = 0; i < COLS; ++i) {
        for (int j = 0; j < ROWS; ++j) {
            std::cout << std::setw(4) << transposed[i][j];
        }
        std::cout << "\n";
    }

    return 0;
}