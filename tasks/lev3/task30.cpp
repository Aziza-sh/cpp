#include <cctype>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>

double evaluateRPN(const std::string &expression) {
  std::stack<double> st;
  std::istringstream iss(expression);
  std::string token;

  while (iss >> token) {
    if (token.size() == 1 &&
        std::string("+-*/").find(token) != std::string::npos) {
      if (st.size() < 2) {
        throw std::runtime_error("Недостаточно операндов");
      }
      double b = st.top();
      st.pop();
      double a = st.top();
      st.pop();
      double result;
      switch (token[0]) {
      case '+':
        result = a + b;
        break;
      case '-':
        result = a - b;
        break;
      case '*':
        result = a * b;
        break;
      case '/':
        if (b == 0)
          throw std::runtime_error("Деление на ноль");
        result = a / b;
        break;
      default:
        throw std::runtime_error("Неизвестный оператор");
      }
      st.push(result);
    } else {
      double num;
      try {
        num = std::stod(token);
      } catch (...) {
        throw std::runtime_error("Некорректный токен: " + token);
      }
      st.push(num);
    }
  }

  if (st.size() != 1) {
    throw std::runtime_error("Неверное выражение");
  }
  return st.top();
}

int main() {
  std::string expr;
  std::cout << "Введите выражение в постфиксной (обратной польской) записи:\n"
            << "Пример: 3 4 + 2 * (это (3+4)*2)\n";
  std::getline(std::cin, expr);

  try {
    double result = evaluateRPN(expr);
    std::cout << "Результат: " << result << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Ошибка: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}