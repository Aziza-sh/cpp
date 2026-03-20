#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>

bool checkBrackets(const std::string &expression) {
  std::stack<char> st;
  std::unordered_map<char, char> matching = {
      {')', '('}, {']', '['}, {'}', '{'}};

  for (char ch : expression) {
    if (ch == '(' || ch == '[' || ch == '{') {
      st.push(ch);
    } else if (ch == ')' || ch == ']' || ch == '}') {
      if (st.empty() || st.top() != matching[ch]) {
        return false;
      }
      st.pop();
    }
  }
  return st.empty();
}

int main() {
  std::string expr;
  std::cout << "Введите выражение для проверки скобок: ";
  std::getline(std::cin, expr);

  if (checkBrackets(expr)) {
    std::cout << "Скобки расставлены правильно.\n";
  } else {
    std::cout << "Скобки расставлены неправильно.\n";
  }

  return 0;
}