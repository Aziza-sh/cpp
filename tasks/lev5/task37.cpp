#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QWidget window;
  window.setWindowTitle("Простая форма");
  window.resize(300, 200);

  QLabel *label = new QLabel("Нажмите кнопку");
  QPushButton *button = new QPushButton("Нажми меня");

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(label);
  layout->addWidget(button);
  window.setLayout(layout);

  QObject::connect(button, &QPushButton::clicked,
                   [label]() { label->setText("Текст изменен!"); });

  window.show();
  return app.exec();
}