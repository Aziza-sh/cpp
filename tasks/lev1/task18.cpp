#include <cmath>
#include <iostream>

const double PI = 3.141592653589793;

double area(double radius) { return PI * radius * radius; }

double area(double width, double height) { return width * height; }

double area(double base, double height, bool isTriangle) {
  return 0.5 * base * height;
}

int main() {
  std::cout << "Площадь круга (r=5): " << area(5.0) << "\n";
  std::cout << "Площадь прямоугольника (4x6): " << area(4.0, 6.0) << "\n";
  std::cout << "Площадь треугольника (основание 4, высота 3): "
            << area(4.0, 3.0, true) << "\n";
  return 0;
}