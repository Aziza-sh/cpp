#include <QApplication>
#include <QFileDialog>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QTextEdit>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
    setWindowTitle("Меню Файл");
    resize(400, 300);

    textEdit = new QTextEdit(this);
    setCentralWidget(textEdit);

    createMenu();
  }

private slots:
  void openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл");
    if (!fileName.isEmpty()) {
      QFile file(fileName);
      if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        textEdit->setPlainText(file.readAll());
        file.close();
      }
    }
  }

  void saveFile() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл");
    if (!fileName.isEmpty()) {
      QFile file(fileName);
      if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(textEdit->toPlainText().toUtf8());
        file.close();
      }
    }
  }

  void exitApp() { QApplication::quit(); }

private:
  void createMenu() {
    QMenu *fileMenu = menuBar()->addMenu("Файл");

    QAction *openAction = new QAction("Открыть", this);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openAction);

    QAction *saveAction = new QAction("Сохранить", this);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    fileMenu->addAction(saveAction);

    fileMenu->addSeparator();

    QAction *exitAction = new QAction("Выход", this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::exitApp);
    fileMenu->addAction(exitAction);
  }

  QTextEdit *textEdit;
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow win;
  win.show();
  return app.exec();
}

#include "task40.moc"