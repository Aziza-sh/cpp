#include <cmath>
#include <iostream>

bool isPrime(int n) {
  if (n <= 1)
    return false;
  for (int i = 2; i <= std::sqrt(n); ++i) {
    if (n % i == 0)
      return false;
  }
  return true;
}

int gcd(int a, int b) {
  while (b != 0) {
    int temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}

double celsiusToFahrenheit(double c) { return c * 9.0 / 5.0 + 32.0; }

int main() {
  int num1 = 17, num2 = 25;
  std::cout << num1 << " простое? " << (isPrime(num1) ? "да" : "нет") << "\n";
  std::cout << "НОД(" << num1 << ", " << num2 << ") = " << gcd(num1, num2)
            << "\n";
  double c = 100.0;
  std::cout << c << "°C = " << celsiusToFahrenheit(c) << "°F\n";
  return 0;
}