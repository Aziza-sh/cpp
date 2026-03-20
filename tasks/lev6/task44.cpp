#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QMessageBox>
#include <vector>
#include <cstdlib>
#include <ctime>

const int CELL_SIZE = 20;
const int WIDTH = 30;
const int HEIGHT = 20;

class SnakeGame : public QWidget {
    Q_OBJECT

public:
    SnakeGame() {
        setFixedSize(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE);
        setWindowTitle("Змейка");
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &SnakeGame::gameTick);
        timer->start(150);
        initGame();
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.fillRect(rect(), Qt::black);
        
        painter.fillRect(food.x() * CELL_SIZE, food.y() * CELL_SIZE, CELL_SIZE, CELL_SIZE, Qt::red);
        for (const QPoint &p : snake) {
            painter.fillRect(p.x() * CELL_SIZE, p.y() * CELL_SIZE, CELL_SIZE, CELL_SIZE, Qt::green);
        }
    }

    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_Up && dir != 2) dir = 0;
        else if (event->key() == Qt::Key_Down && dir != 0) dir = 2;
        else if (event->key() == Qt::Key_Left && dir != 3) dir = 1;
        else if (event->key() == Qt::Key_Right && dir != 1) dir = 3;
    }

private slots:
    void gameTick() {
    
        QPoint newHead = snake[0];
        switch (dir) {
            case 0: newHead.setY(newHead.y() - 1); break;
            case 1: newHead.setX(newHead.x() - 1); break;
            case 2: newHead.setY(newHead.y() + 1); break;
            case 3: newHead.setX(newHead.x() + 1); break;
        }

        if (newHead.x() < 0 || newHead.x() >= WIDTH || newHead.y() < 0 || newHead.y() >= HEIGHT) {
            gameOver();
            return;
        }
        for (const QPoint &p : snake) {
            if (p == newHead) {
                gameOver();
                return;
            }
        }
        snake.insert(snake.begin(), newHead);
        if (newHead == food) {
            score++;
            generateFood();
        } else {
            snake.pop_back();
        }
        update();
    }

private:
    void initGame() {
        snake.clear();
        snake.push_back(QPoint(WIDTH/2, HEIGHT/2));
        dir = 3; 
        score = 0;
        generateFood();
    }

    void generateFood() {
        std::srand(std::time(nullptr));
        do {
            food.setX(std::rand() % WIDTH);
            food.setY(std::rand() % HEIGHT);
        } while (std::find(snake.begin(), snake.end(), food) != snake.end());
    }

    void gameOver() {
        timer->stop();
        QMessageBox::information(this, "Игра окончена", "Ваш счёт: " + QString::number(score));
        initGame();
        timer->start(150);
    }

    std::vector<QPoint> snake;
    QPoint food;
    int dir; 
    int score;
    QTimer *timer;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    SnakeGame game;
    game.show();
    return app.exec();
}

#include "task44.moc"