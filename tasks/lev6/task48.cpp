#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

class ExprParser {
    std::string expr;
    size_t pos;

    double parseExpression();
    double parseTerm();
    double parseFactor();

public:
    ExprParser(const std::string &s) : expr(s), pos(0) {}
    double parse();
};

double ExprParser::parse() {
    return parseExpression();
}

double ExprParser::parseExpression() {
    double result = parseTerm();
    while (pos < expr.size()) {
        char op = expr[pos];
        if (op == '+' || op == '-') {
            pos++;
            double val = parseTerm();
            if (op == '+') result += val;
            else result -= val;
        } else break;
    }
    return result;
}

double ExprParser::parseTerm() {
    double result = parseFactor();
    while (pos < expr.size()) {
        char op = expr[pos];
        if (op == '*' || op == '/') {
            pos++;
            double val = parseFactor();
            if (op == '*') result *= val;
            else {
                if (val == 0) throw std::runtime_error("Деление на ноль");
                result /= val;
            }
        } else break;
    }
    return result;
}

double ExprParser::parseFactor() {
    if (pos >= expr.size()) throw std::runtime_error("Неожиданный конец выражения");
    char ch = expr[pos];
    if (ch == '(') {
        pos++;
        double val = parseExpression();
        if (pos < expr.size() && expr[pos] == ')') pos++;
        else throw std::runtime_error("Ожидалась ')'");
        return val;
    } else if (isdigit(ch) || ch == '.') {
        size_t start = pos;
        while (pos < expr.size() && (isdigit(expr[pos]) || expr[pos] == '.')) pos++;
        std::string numStr = expr.substr(start, pos - start);
        return std::stod(numStr);
    } else {
        throw std::runtime_error(std::string("Неожиданный символ: ") + ch);
    }
}

int main() {
    std::string input;
    std::cout << "Введите арифметическое выражение (+, -, *, /, скобки): ";
    std::getline(std::cin, input);

    try {
        ExprParser parser(input);
        double result = parser.parse();
        std::cout << "Результат: " << result << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}