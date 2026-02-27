#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

class Minesweeper {
  int n, m, mines;
  vector<vector<char>> field;
  vector<vector<bool>> mineMap;
  bool gameOver;

public:
  Minesweeper(int rows, int cols, int mns)
      : n(rows), m(cols), mines(mns), gameOver(false) {
    field.assign(n, vector<char>(m, '#'));
    mineMap.assign(n, vector<bool>(m, false));
    srand(time(0));
    if (mines > n * m)
      throw invalid_argument("Слишком много мин");
    for (int k = 0; k < mines; k++) {
      int i, j;
      do {
        i = rand() % n;
        j = rand() % m;
      } while (mineMap[i][j]);
      mineMap[i][j] = true;
    }
  }

  int countMines(int x, int y) {
    int cnt = 0;
    for (int dx = -1; dx <= 1; dx++)
      for (int dy = -1; dy <= 1; dy++) {
        int nx = x + dx, ny = y + dy;
        if (nx >= 0 && nx < n && ny >= 0 && ny < m && mineMap[nx][ny])
          cnt++;
      }
    return cnt;
  }

  void open(int x, int y) {
    if (x < 0 || x >= n || y < 0 || y >= m || field[x][y] != '#')
      return;
    if (mineMap[x][y]) {
      field[x][y] = '*';
      gameOver = true;
      return;
    }
    int cnt = countMines(x, y);
    field[x][y] = cnt + '0';
    if (cnt == 0)
      for (int dx = -1; dx <= 1; dx++)
        for (int dy = -1; dy <= 1; dy++)
          if (dx != 0 || dy != 0)
            open(x + dx, y + dy);
  }

  void print() {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++)
        cout << field[i][j] << ' ';
      cout << endl;
    }
  }

  bool step() {
    if (gameOver)
      return false;
    int x, y;
    cout << "Введите координаты (строка столбец): ";
    cin >> x >> y;
    if (cin.fail() || x < 0 || x >= n || y < 0 || y >= m) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Некорректные координаты\n";
      return true;
    }
    open(x, y);
    print();
    if (gameOver) {
      cout << "Вы проиграли!\n";
      return false;
    }

    int closed = 0;
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
        if (field[i][j] == '#')
          closed++;
    if (closed == mines) {
      cout << "Поздравляю, вы выиграли!\n";
      return false;
    }
    return true;
  }
};

int main() {
  try {
    Minesweeper game(5, 5, 3);
    game.print();
    while (game.step())
      ;
  } catch (const exception &e) {
    cerr << "Ошибка: " << e.what() << endl;
  }
  return 0;
}