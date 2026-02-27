#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
using namespace std;

struct Record {
  string name;
  int attempts;
};

vector<Record> readRecords() {
  vector<Record> records;
  ifstream f("records.txt");
  if (!f)
    return records;

  Record r;
  while (f >> r.name >> r.attempts) {
    records.push_back(r);
  }
  if (f.bad()) {
    cerr << "Ошибка при чтении файла records.txt\n";
  }
  return records;
}

void saveRecords(const vector<Record> &records) {
  ofstream f("records.txt");
  if (!f) {
    cerr << "Ошибка: не удалось открыть файл для записи\n";
    return;
  }
  for (const auto &r : records) {
    f << r.name << " " << r.attempts << endl;
    if (!f) {
      cerr << "Ошибка записи в файл\n";
      break;
    }
  }
}

void addRecord(const string &name, int attempts) {
  vector<Record> records = readRecords();
  records.push_back({name, attempts});

  sort(records.begin(), records.end(), [](const Record &a, const Record &b) {
    return a.attempts < b.attempts;
  });

  const int MAX_RECORDS = 10;
  if (records.size() > MAX_RECORDS) {
    records.resize(MAX_RECORDS);
  }

  saveRecords(records);
}

void showRecords() {
  vector<Record> records = readRecords();
  if (records.empty()) {
    cout << "Таблица рекордов пуста.\n";
    return;
  }

  sort(records.begin(), records.end(), [](const Record &a, const Record &b) {
    return a.attempts < b.attempts;
  });

  cout << "\n ТАБЛИЦА РЕКОРДОВ  \n";
  cout << "Место | Имя          | Попытки\n";
  int place = 1;
  for (const auto &r : records) {
    cout.width(5);
    cout << left << place << " | ";
    cout.width(12);
    cout << left << r.name << " | " << r.attempts << endl;
    place++;
  }
}

int main() {
  srand(time(0));
  int secret = rand() % 100 + 1;
  int guess, attempts = 0;
  string name;

  cout << "Угадай число (1-100). Введите имя: ";
  getline(cin, name);
  if (name.empty())
    name = "Аноним";

  cout << "Я загадал число от 1 до 100. Попробуй угадать!\n";

  do {
    cout << "Ваш вариант: ";
    cin >> guess;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Ошибка: введите число.\n";
      continue;
    }
    if (guess < 1 || guess > 100) {
      cout << "Число должно быть от 1 до 100.\n";
      continue;
    }
    attempts++;
    if (guess < secret)
      cout << "Загаданное число больше.\n";
    else if (guess > secret)
      cout << "Загаданное число меньше.\n";
  } while (guess != secret);

  cout << "\nПоздравляю, " << name << "! Вы угадали число за " << attempts
       << " попыток.\n\n";

  addRecord(name, attempts);

  showRecords();

  return 0;
}