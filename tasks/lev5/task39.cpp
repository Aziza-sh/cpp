#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <vector>

class DrawingWidget : public QWidget {
public:
  DrawingWidget(QWidget *parent = nullptr) : QWidget(parent) {
    setWindowTitle("Рисование линий");
    setFixedSize(600, 400);
  }

protected:
  void mousePressEvent(QMouseEvent *event) override {
    if (event->button() == Qt::LeftButton) {
      points.push_back(event->pos());
      update();
    }
  }

  void paintEvent(QPaintEvent *event) override {
    QPainter painter(this);
    painter.setPen(Qt::black);
    for (size_t i = 1; i < points.size(); ++i) {
      painter.drawLine(points[i - 1], points[i]);
    }
  }

private:
  std::vector<QPoint> points;
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  DrawingWidget widget;
  widget.show();
  return app.exec();
}