#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <cctype>

struct Row {
    std::map<std::string, std::string> fields;
};

class Table {
public:
    Table(const std::vector<std::string> &cols) : columns(cols) {}
    void insert(const std::vector<std::string> &values);
    void select(const std::vector<std::string> &cols, const std::string &whereCond = "");
    void deleteRows(const std::string &whereCond);
    void saveToFile(const std::string &filename);
    void loadFromFile(const std::string &filename);

private:
    std::vector<std::string> columns;
    std::vector<Row> rows;

    bool evaluateCondition(const Row &row, const std::string &cond);
};

void Table::insert(const std::vector<std::string> &values) {
    if (values.size() != columns.size()) {
        std::cerr << "Ошибка: количество значений не совпадает с количеством колонок\n";
        return;
    }
    Row row;
    for (size_t i = 0; i < columns.size(); ++i) {
        row.fields[columns[i]] = values[i];
    }
    rows.push_back(row);
    std::cout << "Добавлена запись\n";
}

void Table::select(const std::vector<std::string> &cols, const std::string &whereCond) {
    bool all = cols.empty() || (cols.size() == 1 && cols[0] == "*");
    
    if (all) {
        for (const auto &col : columns) std::cout << col << "\t";
    } else {
        for (const auto &col : cols) std::cout << col << "\t";
    }
    std::cout << "\n";
    
    for (const auto &row : rows) {
        if (!whereCond.empty() && !evaluateCondition(row, whereCond)) continue;
        if (all) {
            for (const auto &col : columns) std::cout << row.fields.at(col) << "\t";
        } else {
            for (const auto &col : cols) std::cout << row.fields.at(col) << "\t";
        }
        std::cout << "\n";
    }
}

void Table::deleteRows(const std::string &whereCond) {
    size_t originalSize = rows.size();
    rows.erase(std::remove_if(rows.begin(), rows.end(),
        [&](const Row &row) { return evaluateCondition(row, whereCond); }),
        rows.end());
    std::cout << "Удалено " << (originalSize - rows.size()) << " записей\n";
}

bool Table::evaluateCondition(const Row &row, const std::string &cond) {
    
    size_t eqPos = cond.find('=');
    if (eqPos == std::string::npos) return true; 
    std::string col = cond.substr(0, eqPos);
    std::string val = cond.substr(eqPos + 1);
    
    col.erase(remove_if(col.begin(), col.end(), ::isspace), col.end());
    val.erase(remove_if(val.begin(), val.end(), ::isspace), val.end());
    auto it = row.fields.find(col);
    if (it == row.fields.end()) return false;
    return it->second == val;
}

void Table::saveToFile(const std::string &filename) {
    std::ofstream out(filename);
    if (!out) { std::cerr << "Не удалось сохранить файл\n"; return; }
    
    for (size_t i = 0; i < columns.size(); ++i) {
        out << columns[i];
        if (i != columns.size() - 1) out << ",";
    }
    out << "\n";
    for (const auto &row : rows) {
        for (size_t i = 0; i < columns.size(); ++i) {
            out << row.fields.at(columns[i]);
            if (i != columns.size() - 1) out << ",";
        }
        out << "\n";
    }
    std::cout << "Таблица сохранена\n";
}

void Table::loadFromFile(const std::string &filename) {
    std::ifstream in(filename);
    if (!in) { std::cerr << "Не удалось загрузить файл\n"; return; }
    rows.clear();
    std::string line;
    
    if (!std::getline(in, line)) return;
    std::stringstream ss(line);
    std::string col;
    columns.clear();
    while (std::getline(ss, col, ',')) {
        columns.push_back(col);
    }
    
    while (std::getline(in, line)) {
        std::stringstream data(line);
        std::vector<std::string> values;
        std::string val;
        while (std::getline(data, val, ',')) {
            values.push_back(val);
        }
        if (values.size() == columns.size()) {
            Row row;
            for (size_t i = 0; i < columns.size(); ++i) {
                row.fields[columns[i]] = values[i];
            }
            rows.push_back(row);
        }
    }
    std::cout << "Загружено " << rows.size() << " записей\n";
}

void printHelp() {
    std::cout << "Команды:\n"
              << "  create колонка1 колонка2 ...\n"
              << "  insert значение1 значение2 ...\n"
              << "  select [колонки] [where условие]\n"
              << "  delete where условие\n"
              << "  save файл\n"
              << "  load файл\n"
              << "  exit\n";
}

int main() {
    Table *table = nullptr;
    std::string line;
    printHelp();
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;
        if (cmd == "exit") break;
        else if (cmd == "create") {
            std::vector<std::string> cols;
            std::string col;
            while (ss >> col) cols.push_back(col);
            if (cols.empty()) {
                std::cerr << "Укажите хотя бы одну колонку\n";
            } else {
                delete table;
                table = new Table(cols);
                std::cout << "Таблица создана\n";
            }
        } else if (cmd == "insert") {
            if (!table) { std::cerr << "Сначала создайте таблицу\n"; continue; }
            std::vector<std::string> values;
            std::string val;
            while (ss >> val) values.push_back(val);
            table->insert(values);
        } else if (cmd == "select") {
            if (!table) { std::cerr << "Сначала создайте таблицу\n"; continue; }
            std::vector<std::string> cols;
            std::string token;
            std::string where;
            bool inWhere = false;
            while (ss >> token) {
                if (token == "where") { inWhere = true; continue; }
                if (inWhere) where += (where.empty() ? "" : " ") + token;
                else cols.push_back(token);
            }
            table->select(cols, where);
        } else if (cmd == "delete") {
            if (!table) { std::cerr << "Сначала создайте таблицу\n"; continue; }
            std::string token;
            std::string where;
            ss >> token; 
            std::getline(ss, where);
            if (where.empty()) std::cerr << "Укажите условие (where)\n";
            else table->deleteRows(where);
        } else if (cmd == "save") {
            if (!table) { std::cerr << "Сначала создайте таблицу\n"; continue; }
            std::string filename;
            ss >> filename;
            if (filename.empty()) std::cerr << "Укажите имя файла\n";
            else table->saveToFile(filename);
        } else if (cmd == "load") {
            delete table;
            table = new Table({});
            std::string filename;
            ss >> filename;
            if (filename.empty()) std::cerr << "Укажите имя файла\n";
            else table->loadFromFile(filename);
        } else {
            std::cerr << "Неизвестная команда\n";
        }
    }
    delete table;
    return 0;
}