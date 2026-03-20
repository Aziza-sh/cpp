#include <QApplication>
#include <QColorDialog>
#include <QFontDialog>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>

class DialogDemo : public QWidget {
  Q_OBJECT

public:
  DialogDemo(QWidget *parent = nullptr) : QWidget(parent) {
    setWindowTitle("Диалоги");
    resize(300, 200);

    QVBoxLayout *layout = new QVBoxLayout(this);

    label = new QLabel("Пример текста", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    QPushButton *colorBtn = new QPushButton("Выбрать цвет", this);
    QPushButton *fontBtn = new QPushButton("Выбрать шрифт", this);

    layout->addWidget(colorBtn);
    layout->addWidget(fontBtn);

    connect(colorBtn, &QPushButton::clicked, this, &DialogDemo::chooseColor);
    connect(fontBtn, &QPushButton::clicked, this, &DialogDemo::chooseFont);
  }

private slots:
  void chooseColor() {
    QColor color = QColorDialog::getColor(
        label->palette().color(QPalette::WindowText), this);
    if (color.isValid()) {
      QPalette pal = label->palette();
      pal.setColor(QPalette::WindowText, color);
      label->setPalette(pal);
    }
  }

  void chooseFont() {
    bool ok;
    QFont font = QFontDialog::getFont(&ok, label->font(), this);
    if (ok) {
      label->setFont(font);
    }
  }

private:
  QLabel *label;
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  DialogDemo demo;
  demo.show();
  return app.exec();
}

#include "task41.moc"