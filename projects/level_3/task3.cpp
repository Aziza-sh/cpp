#include "task3.h"
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QGridLayout>
#include <QPushButton>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <QApplication>  

GameBoard::GameBoard() {
    tiles.resize(16);
    for (int i = 0; i < 15; ++i) tiles[i] = i + 1;
    tiles[15] = 0;
    emptyIndex = 15;
}

void GameBoard::shuffle(int moves) {
    std::srand(std::time(nullptr));
    for (int m = 0; m < moves; ++m) {
        int direction = std::rand() % 4;
        int newEmpty = emptyIndex;
        switch (direction) {
            case 0: if (emptyIndex % 4 > 0) newEmpty = emptyIndex - 1; break;
            case 1: if (emptyIndex % 4 < 3) newEmpty = emptyIndex + 1; break;
            case 2: if (emptyIndex / 4 > 0) newEmpty = emptyIndex - 4; break;
            case 3: if (emptyIndex / 4 < 3) newEmpty = emptyIndex + 4; break;
        }
        if (newEmpty != emptyIndex) {
            std::swap(tiles[emptyIndex], tiles[newEmpty]);
            emptyIndex = newEmpty;
        }
    }
}

bool GameBoard::move(int tile) {
    int tileIndex = -1;
    for (int i = 0; i < 16; ++i) {
        if (tiles[i] == tile) {
            tileIndex = i;
            break;
        }
    }
    if (tileIndex == -1) return false;

    int diff = std::abs(tileIndex - emptyIndex);
    if ((diff == 1 && tileIndex / 4 == emptyIndex / 4) || diff == 4) {
        std::swap(tiles[tileIndex], tiles[emptyIndex]);
        emptyIndex = tileIndex;
        return true;
    }
    return false;
}

bool GameBoard::isSolved() const {
    for (int i = 0; i < 15; ++i) {
        if (tiles[i] != i + 1) return false;
    }
    return tiles[15] == 0;
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Пятнашки");
    setFixedSize(400, 450);

    QMenuBar* menuBar = new QMenuBar(this);
    QMenu* gameMenu = menuBar->addMenu("Игра");
    QAction* newGameAction = gameMenu->addAction("Новая игра");
    QAction* exitAction = gameMenu->addAction("Выход");
    setMenuBar(menuBar);

    connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);
    connect(exitAction, &QAction::triggered, this, &MainWindow::exitGame);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(5);

    for (int i = 0; i < 16; ++i) {
        QPushButton* btn = new QPushButton();
        btn->setMinimumSize(80, 80);
        btn->setStyleSheet(
            "font-size: 24px; font-weight: bold; "
            "background: #f8f9fa; border: 2px solid #ccc; "
            "border-radius: 5px;"
        );
        connect(btn, &QPushButton::clicked, this, &MainWindow::onButtonClick);
        buttons.push_back(btn);
        gridLayout->addWidget(btn, i / 4, i % 4);
    }

    board.shuffle();
    updateButtons();
}

MainWindow::~MainWindow() {}

void MainWindow::updateButtons() {
    for (int i = 0; i < 16; ++i) {
        int val = board.getTile(i);
        if (val == 0) {
            buttons[i]->setText("");
            buttons[i]->setEnabled(false);
            buttons[i]->setStyleSheet(
                "font-size: 24px; font-weight: bold; "
                "background: #e0e0e0; border: 2px solid #aaa; "
                "border-radius: 5px;"
            );
        } else {
            buttons[i]->setText(QString::number(val));
            buttons[i]->setEnabled(true);
            buttons[i]->setStyleSheet(
                "font-size: 24px; font-weight: bold; "
                "background: #eb9354; color: white; "
                "border: 2px solid #e07224; border-radius: 5px;"
            );
        }
    }
}

void MainWindow::checkVictory() {
    if (board.isSolved()) {
        QMessageBox::information(this, "Поздравляем!", "Вы собрали головоломку!");
    }
}

void MainWindow::onButtonClick() {
    QPushButton* senderBtn = qobject_cast<QPushButton*>(sender());
    if (!senderBtn) return;

    int index = -1;
    for (int i = 0; i < 16; ++i) {
        if (buttons[i] == senderBtn) {
            index = i;
            break;
        }
    }
    if (index == -1) return;

    int tile = board.getTile(index);
    if (tile != 0 && board.move(tile)) {
        updateButtons();
        checkVictory();
    }
}

void MainWindow::newGame() {
    board.shuffle();
    updateButtons();
}

void MainWindow::exitGame() {
    close();
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}