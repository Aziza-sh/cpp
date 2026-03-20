#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QMenuBar>
#include <QFileDialog>
#include <QFontDialog>
#include <QMessageBox>
#include <QTextDocument>
#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class TextEditor : public QMainWindow {
    Q_OBJECT

public:
    TextEditor() {
        textEdit = new QTextEdit(this);
        setCentralWidget(textEdit);
        createMenus();
        setWindowTitle("Текстовый редактор");
        resize(800, 600);
    }

private slots:
    void openFile() {
        QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Текстовые файлы (*.txt);;Все файлы (*.*)");
        if (fileName.isEmpty()) return;
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
            return;
        }
        textEdit->setPlainText(file.readAll());
        file.close();
    }

    void saveFile() {
        QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "Текстовые файлы (*.txt);;Все файлы (*.*)");
        if (fileName.isEmpty()) return;
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл");
            return;
        }
        file.write(textEdit->toPlainText().toUtf8());
        file.close();
    }

    void changeFont() {
        bool ok;
        QFont font = QFontDialog::getFont(&ok, textEdit->font(), this);
        if (ok) textEdit->setFont(font);
    }

    void findText() {
        findDialog = new QDialog(this);
        findDialog->setWindowTitle("Поиск");
        QVBoxLayout *layout = new QVBoxLayout(findDialog);
        QLineEdit *findEdit = new QLineEdit;
        QPushButton *findBtn = new QPushButton("Найти");
        layout->addWidget(new QLabel("Искать:"));
        layout->addWidget(findEdit);
        layout->addWidget(findBtn);
        connect(findBtn, &QPushButton::clicked, [this, findEdit]() {
            QString text = findEdit->text();
            if (!text.isEmpty()) {
                bool found = textEdit->find(text);
                if (!found) QMessageBox::information(this, "Поиск", "Текст не найден");
            }
        });
        findDialog->show();
    }

private:
    void createMenus() {
        QMenu *fileMenu = menuBar()->addMenu("Файл");
        QAction *openAction = new QAction("Открыть", this);
        connect(openAction, &QAction::triggered, this, &TextEditor::openFile);
        fileMenu->addAction(openAction);

        QAction *saveAction = new QAction("Сохранить", this);
        connect(saveAction, &QAction::triggered, this, &TextEditor::saveFile);
        fileMenu->addAction(saveAction);
        fileMenu->addSeparator();

        QAction *exitAction = new QAction("Выход", this);
        connect(exitAction, &QAction::triggered, this, &QWidget::close);
        fileMenu->addAction(exitAction);

        QMenu *formatMenu = menuBar()->addMenu("Формат");
        QAction *fontAction = new QAction("Шрифт", this);
        connect(fontAction, &QAction::triggered, this, &TextEditor::changeFont);
        formatMenu->addAction(fontAction);

        QMenu *editMenu = menuBar()->addMenu("Правка");
        QAction *findAction = new QAction("Найти", this);
        connect(findAction, &QAction::triggered, this, &TextEditor::findText);
        editMenu->addAction(findAction);
    }

    QTextEdit *textEdit;
    QDialog *findDialog;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    TextEditor editor;
    editor.show();
    return app.exec();
}

#include "task42.moc"