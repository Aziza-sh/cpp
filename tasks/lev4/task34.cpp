#include <cmath>
#include <iostream>

class Shape {
public:
  virtual double area() const = 0; // чисто виртуальный метод
  virtual ~Shape() {}
};

class Circle : public Shape {
private:
  double radius;

public:
  Circle(double r) : radius(r) {}
  double area() const override { return 3.141592653589793 * radius * radius; }
};

class Rectangle : public Shape {
private:
  double width, height;

public:
  Rectangle(double w, double h) : width(w), height(h) {}
  double area() const override { return width * height; }
};

int main() {
  Circle c(5.0);
  Rectangle r(4.0, 6.0);
  std::cout << "Площадь круга: " << c.area() << std::endl;
  std::cout << "Площадь прямоугольника: " << r.area() << std::endl;
  return 0;
}