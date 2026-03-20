#include <iostream>
#include <memory>
#include <vector>

class Shape {
public:
  virtual double area() const = 0;
  virtual ~Shape() {}
};

class Circle : public Shape {
  double radius;

public:
  Circle(double r) : radius(r) {}
  double area() const override { return 3.141592653589793 * radius * radius; }
};

class Rectangle : public Shape {
  double width, height;

public:
  Rectangle(double w, double h) : width(w), height(h) {}
  double area() const override { return width * height; }
};

int main() {
  std::vector<Shape *> shapes;
  shapes.push_back(new Circle(5.0));
  shapes.push_back(new Rectangle(4.0, 6.0));
  shapes.push_back(new Circle(2.5));

  for (const auto &shape : shapes) {
    std::cout << "Площадь: " << shape->area() << std::endl;
  }

  for (auto shape : shapes) {
    delete shape;
  }

  return 0;
}