#ifndef TASK3_H
#define TASK3_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <vector>

class GameBoard {
private:
    std::vector<int> tiles;  
    int emptyIndex;           

public:
    GameBoard();
    void shuffle(int moves = 100);     
    bool move(int tile);               
    bool isSolved() const;             
    int getTile(int index) const { return tiles[index]; }
    int getEmptyIndex() const { return emptyIndex; }
};

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    GameBoard board;
    QWidget* centralWidget;
    QGridLayout* gridLayout;
    std::vector<QPushButton*> buttons; 

    void updateButtons();               
    void checkVictory();                

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected slots:
    void onButtonClick();               
    void newGame();                      
    void exitGame();                  
};

#endif 