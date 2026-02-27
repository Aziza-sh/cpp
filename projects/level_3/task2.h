#ifndef TASK2_H
#define TASK2_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QButtonGroup>
#include <QTimer>
#include <QVBoxLayout>      
#include <QHBoxLayout>     
#include <QString>
#include <vector>

struct Question {
    QString text;
    std::vector<QString> options;
    std::vector<int> correctIndices;
    bool isMultiple;

    Question(const QString& t = "", const std::vector<QString>& opts = {},
             const std::vector<int>& correct = {}, bool multi = false)
        : text(t), options(opts), correctIndices(correct), isMultiple(multi) {}
};

class Test {
private:
    std::vector<Question> questions;
    std::vector<std::vector<int>> userAnswers;
    QString userName;

public:
    void addQuestion(const Question& q);
    void setUserName(const QString& name) { userName = name; }
    QString getUserName() const { return userName; }
    int questionCount() const { return questions.size(); }
    const Question& getQuestion(int index) const { return questions[index]; }
    void setAnswer(int index, const std::vector<int>& selected);
    int calculateScore() const;
    std::vector<QString> getMistakes() const;
};

class StartForm : public QWidget {
    Q_OBJECT
public:
    StartForm(Test& test, QWidget* parent = nullptr);
signals:
    void startTest();
private slots:
    void onStart();
private:
    Test& m_test;
    QLineEdit* nameEdit;
};

class QuestionForm : public QWidget {
    Q_OBJECT
public:
    QuestionForm(Test& test, QWidget* parent = nullptr);
    void startTimer();
signals:
    void finished();
private slots:
    void updateTimer();
    void onNext();
private:
    void showQuestion(int index);
    void clearOptions();
    void saveCurrentAnswer();

    Test& m_test;
    int m_currentIndex;
    QLabel* questionLabel;
    QLabel* questionText;
    QLabel* timerLabel;
    QWidget* optionsContainer;
    QVBoxLayout* optionsLayout;
    std::vector<QWidget*> m_optionWidgets;
    std::vector<QButtonGroup*> m_buttonGroups;
    QTimer* timer;
    int m_timeLeft;
};

class ResultForm : public QWidget {
    Q_OBJECT
public:
    ResultForm(Test& test, QWidget* parent = nullptr);
};

class TestController : public QObject {
    Q_OBJECT
public:
    TestController();
private slots:
    void showStartForm();
    void showQuestionForm();
    void showResultForm();
private:
    Test m_test;
    StartForm* startForm = nullptr;
    QuestionForm* questionForm = nullptr;
    ResultForm* resultForm = nullptr;
};

#endif 