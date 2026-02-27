#include <iostream>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>

using namespace std;

double evalPostfix(const string &expr) {
  stack<double> st;
  istringstream iss(expr);
  string token;

  while (iss >> token) {

    try {
      size_t pos;
      double num = stod(token, &pos);

      if (pos == token.length()) {
        st.push(num);
        continue;
      }

    } catch (const invalid_argument &) {

    } catch (const out_of_range &) {
      throw runtime_error("Число вне допустимого диапазона: " + token);
    }

    if (st.size() < 2) {
      throw runtime_error("Недостаточно операндов для операции " + token);
    }

    double b = st.top();
    st.pop();
    double a = st.top();
    st.pop();

    if (token == "+") {
      st.push(a + b);
    } else if (token == "-") {
      st.push(a - b);
    } else if (token == "*") {
      st.push(a * b);
    } else if (token == "/") {
      if (b == 0)
        throw runtime_error("Деление на ноль");
      st.push(a / b);
    } else {
      throw runtime_error("Неизвестная операция: " + token);
    }
  }

  if (st.size() != 1) {
    throw runtime_error("Некорректное выражение: лишние операнды");
  }
  return st.top();
}

int main() {
  try {
    cout << "Введите постфиксное выражение (например, '5 3 + 2 *'): ";
    string expr;
    getline(cin, expr);

    if (expr.empty()) {
      cerr << "Ошибка: пустая строка." << endl;
      return 1;
    }

    double result = evalPostfix(expr);
    cout << "Результат: " << result << endl;

  } catch (const exception &e) {
    cerr << "Ошибка: " << e.what() << endl;
  }
  return 0;
}