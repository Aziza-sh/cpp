#include <iostream>
#include <limits>
#include <string>
using namespace std;

struct Task {
  string desc;
  bool done;
  Task *next;
  Task(string d) : desc(d), done(false), next(nullptr) {}
};

class TodoList {
  Task *head;

public:
  TodoList() : head(nullptr) {}
  ~TodoList() {
    while (head) {
      Task *t = head;
      head = head->next;
      delete t;
    }
  }

  void addFront(string d) {
    if (d.empty()) {
      cout << "Описание не может быть пустым\n";
      return;
    }
    Task *t = new Task(d);
    t->next = head;
    head = t;
  }

  void addBack(string d) {
    if (d.empty()) {
      cout << "Описание не может быть пустым\n";
      return;
    }
    Task *t = new Task(d);
    if (!head)
      head = t;
    else {
      Task *cur = head;
      while (cur->next)
        cur = cur->next;
      cur->next = t;
    }
  }

  void remove(string d) {
    if (d.empty()) {
      cout << "Введите описание\n";
      return;
    }
    Task *prev = nullptr, *cur = head;
    while (cur && cur->desc != d) {
      prev = cur;
      cur = cur->next;
    }
    if (!cur) {
      cout << "Задача не найдена\n";
      return;
    }
    if (!prev)
      head = cur->next;
    else
      prev->next = cur->next;
    delete cur;
    cout << "Задача удалена\n";
  }

  void markDone(string d) {
    if (d.empty()) {
      cout << "Введите описание\n";
      return;
    }
    Task *cur = head;
    while (cur && cur->desc != d)
      cur = cur->next;
    if (cur) {
      cur->done = true;
      cout << "Задача отмечена выполненной\n";
    } else
      cout << "Задача не найдена\n";
  }

  void print() {
    if (!head) {
      cout << "Список пуст\n";
      return;
    }
    for (Task *cur = head; cur; cur = cur->next)
      cout << (cur->done ? "[X] " : "[ ] ") << cur->desc << endl;
  }
};

int main() {
  TodoList list;
  int choice;
  string desc;
  do {
    cout << "\n1. Добавить в начало\n2. Добавить в конец\n3. Удалить\n4. "
            "Отметить выполненным\n5. Показать все\n0. Выход\n";
    cin >> choice;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Некорректный ввод\n";
      continue;
    }
    cin.ignore();
    switch (choice) {
    case 1:
      cout << "Описание: ";
      getline(cin, desc);
      list.addFront(desc);
      break;
    case 2:
      cout << "Описание: ";
      getline(cin, desc);
      list.addBack(desc);
      break;
    case 3:
      cout << "Описание: ";
      getline(cin, desc);
      list.remove(desc);
      break;
    case 4:
      cout << "Описание: ";
      getline(cin, desc);
      list.markDone(desc);
      break;
    case 5:
      list.print();
      break;
    case 0:
      break;
    default:
      cout << "Неверный пункт\n";
    }
  } while (choice != 0);
  return 0;
}