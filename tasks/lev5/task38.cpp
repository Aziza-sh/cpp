include<QApplication>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QWidget>

    class Calculator : public QWidget {
  Q_OBJECT

public:
  Calculator(QWidget *parent = nullptr) : QWidget(parent) {
    display = new QLineEdit(this);
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setText("0");

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(display, 0, 0, 1, 4);

    const char *buttons[4][4] = {{"7", "8", "9", "/"},
                                 {"4", "5", "6", "*"},
                                 {"1", "2", "3", "-"},
                                 {"0", "C", "=", "+"}};

    for (int row = 0; row < 4; ++row) {
      for (int col = 0; col < 4; ++col) {
        QPushButton *btn = new QPushButton(buttons[row][col], this);
        layout->addWidget(btn, row + 1, col);
        connect(btn, &QPushButton::clicked, this, &Calculator::onButtonClicked);
      }
    }

    setLayout(layout);
    setWindowTitle("Калькулятор");
  }

private slots:
  void onButtonClicked() {
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (!btn)
      return;
    QString text = btn->text();

    if (text == "C") {
      display->clear();
      currentNumber.clear();
      currentOperator.clear();
      waitingForOperand = false;
    } else if (text == "=") {
      if (!currentOperator.isEmpty() && !currentNumber.isEmpty()) {
        double result = calculate(display->text().toDouble(),
                                  currentNumber.toDouble(), currentOperator);
        display->setText(QString::number(result));
        currentNumber.clear();
        currentOperator.clear();
        waitingForOperand = true;
      }
    } else if (text == "+" || text == "-" || text == "*" || text == "/") {
      if (!waitingForOperand) {
        currentNumber = display->text();
        currentOperator = text;
        waitingForOperand = true;
      } else {
        // Если уже есть оператор, вычисляем предыдущее выражение
        double result = calculate(display->text().toDouble(),
                                  currentNumber.toDouble(), currentOperator);
        display->setText(QString::number(result));
        currentNumber = QString::number(result);
        currentOperator = text;
      }
    } else {
      // цифры
      if (waitingForOperand) {
        display->clear();
        waitingForOperand = false;
      }
      if (display->text() == "0")
        display->setText(text);
      else
        display->setText(display->text() + text);
    }
  }

private:
  double calculate(double a, double b, const QString &op) {
    if (op == "+")
      return a + b;
    if (op == "-")
      return a - b;
    if (op == "*")
      return a * b;
    if (op == "/")
      return a / b;
    return 0;
  }

  QLineEdit *display;
  QString currentNumber;
  QString currentOperator;
  bool waitingForOperand = false;
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  Calculator calc;
  calc.show();
  return app.exec();
}

#include "task38.moc"