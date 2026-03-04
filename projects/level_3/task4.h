#ifndef TASK4_H
#define TASK4_H

#include <QMainWindow>
#include <QAbstractTableModel>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QString>
#include <vector>

struct Contact {
    QString name;
    QString phone;
    QString email;
    Contact() = default;
    Contact(const QString& n, const QString& p, const QString& e) : name(n), phone(p), email(e) {}
};

class NotebookService {
public:
    NotebookService(const QString& filename = "contacts.txt");
    bool load();
    bool save();
    void addContact(const Contact& c);
    bool updateContact(int index, const Contact& c);
    bool deleteContact(int index);
    const Contact& getContact(int index) const;
    int count() const { return contacts.size(); }
    std::vector<Contact> searchByName(const QString& name) const;
private:
    QString filename;
    std::vector<Contact> contacts;
};

class ContactTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    ContactTableModel(NotebookService& service, QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void refresh();
private:
    NotebookService& service;
};

class ContactDialog : public QDialog {
    Q_OBJECT
public:
    ContactDialog(QWidget* parent = nullptr);
    void setContact(const Contact& c);
    Contact getContact() const;
private:
    QLineEdit* nameEdit;
    QLineEdit* phoneEdit;
    QLineEdit* emailEdit;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
protected slots:
    void onAdd();
    void onEdit();
    void onDelete();
    void onSearch();
    void refreshView();
private:
    NotebookService service;
    ContactTableModel* model;
    QTableView* tableView;
    QLineEdit* searchEdit;
    QPushButton* searchButton;
};

#endif 