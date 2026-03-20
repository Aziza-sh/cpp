#include <QApplication>
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <vector>
#include <string>

struct Student {
    QString name;
    int grade;
};

class StudentDB : public QMainWindow {
    Q_OBJECT

public:
    StudentDB() {
        setWindowTitle("База данных студентов");
        resize(600, 400);

        QWidget *central = new QWidget(this);
        setCentralWidget(central);
        QVBoxLayout *mainLayout = new QVBoxLayout(central);

        // Таблица
        table = new QTableWidget(0, 2, this);
        table->setHorizontalHeaderLabels({"Имя", "Оценка"});
        table->horizontalHeader()->setStretchLastSection(true);
        mainLayout->addWidget(table);

        // Панель ввода
        QHBoxLayout *inputLayout = new QHBoxLayout;
        nameEdit = new QLineEdit;
        nameEdit->setPlaceholderText("Имя студента");
        gradeEdit = new QLineEdit;
        gradeEdit->setPlaceholderText("Оценка");
        QPushButton *addBtn = new QPushButton("Добавить");
        QPushButton *editBtn = new QPushButton("Редактировать");
        QPushButton *deleteBtn = new QPushButton("Удалить");
        QPushButton *searchBtn = new QPushButton("Поиск");
        QPushButton *reportBtn = new QPushButton("Отчёт");
        QPushButton *saveBtn = new QPushButton("Сохранить");
        QPushButton *loadBtn = new QPushButton("Загрузить");

        inputLayout->addWidget(nameEdit);
        inputLayout->addWidget(gradeEdit);
        inputLayout->addWidget(addBtn);
        inputLayout->addWidget(editBtn);
        inputLayout->addWidget(deleteBtn);
        inputLayout->addWidget(searchBtn);
        inputLayout->addWidget(reportBtn);
        inputLayout->addWidget(saveBtn);
        inputLayout->addWidget(loadBtn);
        mainLayout->addLayout(inputLayout);

        connect(addBtn, &QPushButton::clicked, this, &StudentDB::addStudent);
        connect(editBtn, &QPushButton::clicked, this, &StudentDB::editStudent);
        connect(deleteBtn, &QPushButton::clicked, this, &StudentDB::deleteStudent);
        connect(searchBtn, &QPushButton::clicked, this, &StudentDB::searchStudent);
        connect(reportBtn, &QPushButton::clicked, this, &StudentDB::generateReport);
        connect(saveBtn, &QPushButton::clicked, this, &StudentDB::saveToFile);
        connect(loadBtn, &QPushButton::clicked, this, &StudentDB::loadFromFile);
    }

private slots:
    void addStudent() {
        QString name = nameEdit->text().trimmed();
        if (name.isEmpty()) return;
        bool ok;
        int grade = gradeEdit->text().toInt(&ok);
        if (!ok || grade < 2 || grade > 5) {
            QMessageBox::warning(this, "Ошибка", "Оценка должна быть от 2 до 5");
            return;
        }
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(name));
        table->setItem(row, 1, new QTableWidgetItem(QString::number(grade)));
        nameEdit->clear();
        gradeEdit->clear();
    }

    void editStudent() {
        int row = table->currentRow();
        if (row < 0) return;
        QString name = nameEdit->text().trimmed();
        bool ok;
        int grade = gradeEdit->text().toInt(&ok);
        if (!name.isEmpty() && ok && grade >= 2 && grade <= 5) {
            table->item(row, 0)->setText(name);
            table->item(row, 1)->setText(QString::number(grade));
        } else {
            QMessageBox::warning(this, "Ошибка", "Введите корректные данные");
        }
    }

    void deleteStudent() {
        int row = table->currentRow();
        if (row >= 0) table->removeRow(row);
    }

    void searchStudent() {
        QString searchName = nameEdit->text().trimmed();
        if (searchName.isEmpty()) return;
        for (int row = 0; row < table->rowCount(); ++row) {
            if (table->item(row, 0)->text().contains(searchName, Qt::CaseInsensitive)) {
                table->selectRow(row);
                table->scrollToItem(table->item(row, 0));
                return;
            }
        }
        QMessageBox::information(this, "Поиск", "Студент не найден");
    }

    void generateReport() {
        int count = table->rowCount();
        double sum = 0;
        int perfect = 0, fail = 0;
        for (int i = 0; i < count; ++i) {
            int grade = table->item(i, 1)->text().toInt();
            sum += grade;
            if (grade == 5) perfect++;
            if (grade == 2) fail++;
        }
        double avg = count ? sum / count : 0;
        QString report = QString("Всего студентов: %1\nСредний балл: %2\nОтличников (5): %3\nДвоечников (2): %4")
                         .arg(count).arg(avg, 0, 'f', 2).arg(perfect).arg(fail);
        QMessageBox::information(this, "Отчёт", report);
    }

    void saveToFile() {
        QString fileName = QFileDialog::getSaveFileName(this, "Сохранить базу", "", "CSV файлы (*.csv)");
        if (fileName.isEmpty()) return;
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
        QTextStream out(&file);
        for (int i = 0; i < table->rowCount(); ++i) {
            out << table->item(i, 0)->text() << "," << table->item(i, 1)->text() << "\n";
        }
        file.close();
    }

    void loadFromFile() {
        QString fileName = QFileDialog::getOpenFileName(this, "Загрузить базу", "", "CSV файлы (*.csv)");
        if (fileName.isEmpty()) return;
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
        table->setRowCount(0);
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(',');
            if (fields.size() >= 2) {
                int row = table->rowCount();
                table->insertRow(row);
                table->setItem(row, 0, new QTableWidgetItem(fields[0]));
                table->setItem(row, 1, new QTableWidgetItem(fields[1]));
            }
        }
        file.close();
    }

private:
    QTableWidget *table;
    QLineEdit *nameEdit, *gradeEdit;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    StudentDB db;
    db.show();
    return app.exec();
}

#include "task43.moc"