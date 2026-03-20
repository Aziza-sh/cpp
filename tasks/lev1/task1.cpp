#include <iostream>

int main() {
    int a = 5;
    double b = 2.3;
    auto result = a + b; 
    std::cout << "Неявное преобразование: 5 + 2.3 = " << result << " (тип double)\n";

    double pi = 3.14159;
    int intPi = static_cast<int>(pi);
    std::cout << "Явное преобразование (double -> int): " << intPi << "\n";

    float large = 12345.67f;
    int truncated = static_cast<int>(large);
    std::cout << "Потеря данных: float " << large << " -> int " << truncated << "\n";

    return 0;
}