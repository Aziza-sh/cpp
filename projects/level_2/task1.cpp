#include "rle.h"
#include <iostream>
using namespace std;

int main() {
  try {
    string text = "aaaaaaaaaaaaannnnnnnnnnnnnddddddddddd";
    string c = compress(text);
    cout << "Сжато: " << c << " (длина " << c.size() << ")\n";
    cout << "Коэф. сжатия: " << (double)text.size() / c.size() << endl;
    string d = decompress(c);
    cout << "Восстановлено: " << d << endl;
    if (d != text)
      throw runtime_error("Восстановленная строка не совпадает");
  } catch (const exception &e) {
    cerr << "Ошибка: " << e.what() << endl;
  }
  return 0;
}