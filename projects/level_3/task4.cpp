#include "task4.h"
#include <QApplication>
#include <QFile>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QTextStream>
#include <QVBoxLayout>

NotebookService::NotebookService(const QString &fname) : filename(fname) {
  load();
}

bool NotebookService::load() {
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;
  QTextStream in(&file);
  contacts.clear();
  while (!in.atEnd()) {
    QString line = in.readLine();
    QStringList parts = line.split(';');
    if (parts.size() == 3) {
      contacts.emplace_back(parts[0], parts[1], parts[2]);
    }
  }
  file.close();
  return true;
}

bool NotebookService::save() {
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return false;
  QTextStream out(&file);
  for (const auto &c : contacts) {
    out << c.name << ";" << c.phone << ";" << c.email << "\n";
  }
  file.close();
  return true;
}

void NotebookService::addContact(const Contact &c) {
  contacts.push_back(c);
  save();
}

bool NotebookService::updateContact(int index, const Contact &c) {
  if (index < 0 || index >= (int)contacts.size())
    return false;
  contacts[index] = c;
  save();
  return true;
}

bool NotebookService::deleteContact(int index) {
  if (index < 0 || index >= (int)contacts.size())
    return false;
  contacts.erase(contacts.begin() + index);
  save();
  return true;
}

const Contact &NotebookService::getContact(int index) const {
  static Contact empty;
  if (index < 0 || index >= (int)contacts.size())
    return empty;
  return contacts[index];
}

std::vector<Contact> NotebookService::searchByName(const QString &name) const {
  std::vector<Contact> result;
  for (const auto &c : contacts) {
    if (c.name.contains(name, Qt::CaseInsensitive))
      result.push_back(c);
  }
  return result;
}

ContactTableModel::ContactTableModel(NotebookService &s, QObject *parent)
    : QAbstractTableModel(parent), service(s) {}

int ContactTableModel::rowCount(const QModelIndex &) const {
  return service.count();
}

int ContactTableModel::columnCount(const QModelIndex &) const { return 3; }

QVariant ContactTableModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || role != Qt::DisplayRole)
    return QVariant();
  const Contact &c = service.getContact(index.row());
  switch (index.column()) {
  case 0:
    return c.name;
  case 1:
    return c.phone;
  case 2:
    return c.email;
  default:
    return QVariant();
  }
}

QVariant ContactTableModel::headerData(int section, Qt::Orientation orientation,
                                       int role) const {
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
    return QVariant();
  switch (section) {
  case 0:
    return "Имя";
  case 1:
    return "Телефон";
  case 2:
    return "Email";
  default:
    return QVariant();
  }
}

void ContactTableModel::refresh() {
  beginResetModel();
  endResetModel();
}

ContactDialog::ContactDialog(QWidget *parent) : QDialog(parent) {
  setWindowTitle("Контакт");
  QFormLayout *layout = new QFormLayout(this);

  nameEdit = new QLineEdit;
  phoneEdit = new QLineEdit;
  emailEdit = new QLineEdit;

  layout->addRow("Имя:", nameEdit);
  layout->addRow("Телефон:", phoneEdit);
  layout->addRow("Email:", emailEdit);

  QPushButton *okButton = new QPushButton("OK");
  QPushButton *cancelButton = new QPushButton("Отмена");
  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);
  layout->addRow(buttonLayout);

  connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void ContactDialog::setContact(const Contact &c) {
  nameEdit->setText(c.name);
  phoneEdit->setText(c.phone);
  emailEdit->setText(c.email);
}

Contact ContactDialog::getContact() const {
  return Contact(nameEdit->text(), phoneEdit->text(), emailEdit->text());
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Записная книжка");
  resize(600, 400);

  setStyleSheet(
      "QMainWindow { background-color: #FFF5E6; }"
      "QPushButton { background-color: #FF8C00; color: white; border: none; "
      "padding: 5px; border-radius: 3px; font-weight: bold; }"
      "QPushButton:hover { background-color: #FFA500; }"
      "QPushButton:pressed { background-color: #E07B00; }"
      "QHeaderView::section { background-color: #FF8C00; color: white; "
      "padding: 4px; border: 1px solid #E07B00; }"
      "QTableView::item:selected { background-color: #FFD700; color: black; }"
      "QTableView { gridline-color: #FFA500; }"
      "QLineEdit { border: 1px solid #FF8C00; border-radius: 3px; padding: "
      "3px; }"
      "QLineEdit:focus { border: 2px solid #FF8C00; }"
      "QLabel { color: #8B4513; }");

  QWidget *central = new QWidget;
  setCentralWidget(central);
  QVBoxLayout *mainLayout = new QVBoxLayout(central);

  QHBoxLayout *searchLayout = new QHBoxLayout;
  searchLayout->addWidget(new QLabel("Поиск:"));
  searchEdit = new QLineEdit;
  searchLayout->addWidget(searchEdit);
  searchButton = new QPushButton("Найти");
  searchLayout->addWidget(searchButton);
  mainLayout->addLayout(searchLayout);

  model = new ContactTableModel(service, this);
  tableView = new QTableView;
  tableView->setModel(model);
  tableView->horizontalHeader()->setStretchLastSection(true);
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  mainLayout->addWidget(tableView);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  QPushButton *addButton = new QPushButton("Добавить");
  QPushButton *editButton = new QPushButton("Редактировать");
  QPushButton *deleteButton = new QPushButton("Удалить");
  QPushButton *refreshButton = new QPushButton("Обновить");
  buttonLayout->addWidget(addButton);
  buttonLayout->addWidget(editButton);
  buttonLayout->addWidget(deleteButton);
  buttonLayout->addWidget(refreshButton);
  mainLayout->addLayout(buttonLayout);

  connect(addButton, &QPushButton::clicked, this, &MainWindow::onAdd);
  connect(editButton, &QPushButton::clicked, this, &MainWindow::onEdit);
  connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDelete);
  connect(refreshButton, &QPushButton::clicked, this, &MainWindow::refreshView);
  connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearch);
  connect(searchEdit, &QLineEdit::returnPressed, this, &MainWindow::onSearch);
}

MainWindow::~MainWindow() {}

void MainWindow::onAdd() {
  ContactDialog dlg(this);
  if (dlg.exec() == QDialog::Accepted) {
    service.addContact(dlg.getContact());
    refreshView();
  }
}

void MainWindow::onEdit() {
  QModelIndex current = tableView->currentIndex();
  if (!current.isValid()) {
    QMessageBox::warning(this, "Ошибка",
                         "Выберите контакт для редактирования.");
    return;
  }
  int row = current.row();
  const Contact &c = service.getContact(row);
  ContactDialog dlg(this);
  dlg.setContact(c);
  if (dlg.exec() == QDialog::Accepted) {
    service.updateContact(row, dlg.getContact());
    refreshView();
  }
}

void MainWindow::onDelete() {
  QModelIndex current = tableView->currentIndex();
  if (!current.isValid())
    return;
  int row = current.row();
  if (QMessageBox::question(this, "Подтверждение", "Удалить контакт?") ==
      QMessageBox::Yes) {
    service.deleteContact(row);
    refreshView();
  }
}

void MainWindow::onSearch() {
  QString query = searchEdit->text().trimmed();
  if (query.isEmpty()) {
    refreshView();
    return;
  }
  auto results = service.searchByName(query);
  QStringList lines;
  for (const auto &c : results) {
    lines << QString("%1 | %2 | %3").arg(c.name).arg(c.phone).arg(c.email);
  }
  if (lines.isEmpty())
    QMessageBox::information(this, "Результат", "Ничего не найдено.");
  else
    QMessageBox::information(this, "Результаты поиска", lines.join("\n"));
}

void MainWindow::refreshView() { model->refresh(); }

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow w;
  w.show();
  return app.exec();
}