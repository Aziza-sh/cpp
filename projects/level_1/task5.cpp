#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

struct Polynomial {
  vector<double> coeff;
};

Polynomial inputPolynomial() {
  int n;
  cout << "Введите степень многочлена: ";
  cin >> n;
  if (n < 0)
    throw invalid_argument("Степень не может быть отрицательной");
  Polynomial p;
  p.coeff.resize(n + 1);
  cout << "Введите " << n + 1 << " коэффициентов (от a0 до a" << n << "): ";
  for (int i = 0; i <= n; ++i) {
    cin >> p.coeff[i];
  }
  return p;
}

void printPolynomial(const Polynomial &p) {
  if (p.coeff.empty()) {
    cout << "0";
    return;
  }
  bool first = true;
  for (int i = static_cast<int>(p.coeff.size()) - 1; i >= 0; --i) {
    double c = p.coeff[i];
    if (c == 0.0)
      continue;
    if (!first) {
      cout << (c > 0 ? " + " : " - ");
    } else {
      if (c < 0)
        cout << "-";
      first = false;
    }
    double abs_c = abs(c);
    if (i == 0) {
      cout << abs_c;
    } else {
      if (abs_c != 1.0)
        cout << abs_c;
      cout << "x";
      if (i > 1)
        cout << "^" << i;
    }
  }
  if (first)
    cout << "0";
}

double at(const Polynomial &p, int i) {
  if (i < 0 || i >= static_cast<int>(p.coeff.size()))
    return 0.0;
  return p.coeff[i];
}

Polynomial add(const Polynomial &a, const Polynomial &b) {
  int n = max(a.coeff.size(), b.coeff.size());
  Polynomial res;
  res.coeff.resize(n);
  for (int i = 0; i < n; ++i) {
    res.coeff[i] = at(a, i) + at(b, i);
  }
  return res;
}

// Вычитание многочленов
Polynomial subtract(const Polynomial &a, const Polynomial &b) {
  int n = max(a.coeff.size(), b.coeff.size());
  Polynomial res;
  res.coeff.resize(n);
  for (int i = 0; i < n; ++i) {
    res.coeff[i] = at(a, i) - at(b, i);
  }
  return res;
}

Polynomial multiply(const Polynomial &a, const Polynomial &b) {
  if (a.coeff.empty() || b.coeff.empty())
    return Polynomial{};
  Polynomial res;
  res.coeff.resize(a.coeff.size() + b.coeff.size() - 1, 0.0);
  for (size_t i = 0; i < a.coeff.size(); ++i) {
    for (size_t j = 0; j < b.coeff.size(); ++j) {
      res.coeff[i + j] += a.coeff[i] * b.coeff[j];
    }
  }
  return res;
}

double hornerRecursive(const vector<double> &coeff, int index, double x,
                       double acc) {
  if (index < 0)
    return acc;
  return hornerRecursive(coeff, index - 1, x, acc * x + coeff[index]);
}

double horner(const Polynomial &p, double x) {
  if (p.coeff.empty())
    return 0.0;
  return hornerRecursive(p.coeff, static_cast<int>(p.coeff.size()) - 1, x, 0.0);
}

int main() {
  try {
    int choice;
    do {
      cout << "\nМеню:\n";
      cout << "1. Сложение многочленов\n";
      cout << "2. Вычитание многочленов\n";
      cout << "3. Умножение многочленов\n";
      cout << "4. Выход\n";
      cout << "Выберите операцию: ";
      cin >> choice;

      if (choice >= 1 && choice <= 3) {
        cout << "Введите первый многочлен:\n";
        Polynomial p1 = inputPolynomial();
        cout << "Введите второй многочлен:\n";
        Polynomial p2 = inputPolynomial();

        Polynomial result;
        string opName;
        switch (choice) {
        case 1:
          result = add(p1, p2);
          opName = "Сумма";
          break;
        case 2:
          result = subtract(p1, p2);
          opName = "Разность";
          break;
        case 3:
          result = multiply(p1, p2);
          opName = "Произведение";
          break;
        }

        cout << opName << " многочленов: ";
        printPolynomial(result);
        cout << endl;

        char calc;
        cout
            << "Вычислить значение результирующего многочлена в точке? (y/n): ";
        cin >> calc;
        if (calc == 'y' || calc == 'Y') {
          double x;
          cout << "Введите x: ";
          cin >> x;
          double val = horner(result, x);
          cout << "Значение при x = " << x << " равно " << val << endl;
        }
      }
    } while (choice != 4);

  } catch (const exception &e) {
    cerr << "Ошибка: " << e.what() << endl;
  }
  return 0;
}