#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
using namespace std;

struct Book {
  string author, title;
  int year;
  bool available;
};

vector<Book> catalog;

void save() {
  ofstream f("catalog.dat", ios::binary);
  if (!f) {
    cerr << "Ошибка: не удалось открыть файл для записи\n";
    return;
  }
  for (const auto &b : catalog) {
    size_t len = b.author.size();
    f.write((char *)&len, sizeof(len));
    f.write(b.author.c_str(), len);
    len = b.title.size();
    f.write((char *)&len, sizeof(len));
    f.write(b.title.c_str(), len);
    f.write((char *)&b.year, sizeof(b.year));
    f.write((char *)&b.available, sizeof(b.available));
    if (!f) {
      cerr << "Ошибка записи\n";
      break;
    }
  }
}

void load() {
  ifstream f("catalog.dat", ios::binary);
  if (!f)
    return;
  catalog.clear();
  while (f) {
    Book b;
    size_t len;
    if (!f.read((char *)&len, sizeof(len)))
      break;
    if (len > 1000) {
      cerr << "Ошибка формата файла\n";
      break;
    }
    b.author.resize(len);
    f.read(&b.author[0], len);
    f.read((char *)&len, sizeof(len));
    if (len > 1000)
      break;
    b.title.resize(len);
    f.read(&b.title[0], len);
    f.read((char *)&b.year, sizeof(b.year));
    f.read((char *)&b.available, sizeof(b.available));
    if (f)
      catalog.push_back(b);
  }
  if (f.bad())
    cerr << "Ошибка при чтении файла\n";
}

void add() {
  Book b;
  cout << "Автор: ";
  cin.ignore();
  getline(cin, b.author);
  if (b.author.empty()) {
    cout << "Автор не может быть пустым\n";
    return;
  }
  cout << "Название: ";
  getline(cin, b.title);
  if (b.title.empty()) {
    cout << "Название не может быть пустым\n";
    return;
  }
  cout << "Год: ";
  cin >> b.year;
  if (cin.fail() || b.year < 0 || b.year > 2100) {
    cout << "Некорректный год\n";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return;
  }
  b.available = true;
  catalog.push_back(b);
  save();
  cout << "Книга добавлена.\n";
}

void searchByAuthor() {
  string auth;
  cout << "Введите автора: ";
  cin.ignore();
  getline(cin, auth);
  if (auth.empty()) {
    cout << "Введите автора\n";
    return;
  }
  bool found = false;
  for (const auto &b : catalog) {
    if (b.author == auth) {
      cout << " - " << b.title << " (" << b.year << ") "
           << (b.available ? "доступна" : "выдана") << endl;
      found = true;
    }
  }
  if (!found)
    cout << "Книги этого автора не найдены\n";
}

void searchByTitle() {
  string title;
  cout << "Введите название: ";
  cin.ignore();
  getline(cin, title);
  if (title.empty()) {
    cout << "Введите название\n";
    return;
  }
  bool found = false;
  for (const auto &b : catalog) {
    if (b.title == title) {
      cout << "Автор: " << b.author << ", год: " << b.year
           << ", статус: " << (b.available ? "доступна" : "выдана") << endl;
      found = true;
    }
  }
  if (!found)
    cout << "Книга с таким названием не найдена\n";
}

void displayAll() {
  if (catalog.empty()) {
    cout << "Каталог пуст.\n";
    return;
  }
  cout << "\nКаталог\n";
  for (size_t i = 0; i < catalog.size(); i++) {
    const auto &b = catalog[i];
    cout << i + 1 << ". " << b.author << " – \"" << b.title << "\" (" << b.year
         << ") – " << (b.available ? "доступна" : "выдана") << endl;
  }
}

void checkout() {
  string title;
  cout << "Введите название книги для выдачи: ";
  cin.ignore();
  getline(cin, title);
  if (title.empty()) {
    cout << "Введите название\n";
    return;
  }
  for (auto &b : catalog) {
    if (b.title == title) {
      if (b.available) {
        b.available = false;
        save();
        cout << "Книга выдана.\n";
      } else {
        cout << "Книга уже выдана.\n";
      }
      return;
    }
  }
  cout << "Книга не найдена.\n";
}

int main() {
  load();
  int choice;
  do {
    cout << "\nБиблиотека\n";
    cout << "1. Добавить книгу\n";
    cout << "2. Поиск по автору\n";
    cout << "3. Поиск по названию\n";
    cout << "4. Выдать книгу\n";
    cout << "5. Показать весь каталог\n";
    cout << "0. Выход\n";
    cout << "Выберите пункт: ";
    cin >> choice;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Некорректный ввод\n";
      continue;
    }
    switch (choice) {
    case 1:
      add();
      break;
    case 2:
      searchByAuthor();
      break;
    case 3:
      searchByTitle();
      break;
    case 4:
      checkout();
      break;
    case 5:
      displayAll();
      break;
    case 0:
      break;
    default:
      cout << "Неверный пункт\n";
    }
  } while (choice != 0);
  return 0;
}