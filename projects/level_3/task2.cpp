#include "task2.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <algorithm>

void Test::addQuestion(const Question& q) {
    questions.push_back(q);
    userAnswers.resize(questions.size());
}

void Test::setAnswer(int index, const std::vector<int>& selected) {
    if (index >= 0 && index < (int)userAnswers.size())
        userAnswers[index] = selected;
}

int Test::calculateScore() const {
    int correct = 0;
    for (size_t i = 0; i < questions.size(); ++i) {
        const auto& q = questions[i];
        const auto& ans = userAnswers[i];
        std::vector<int> sortedAns = ans;
        std::vector<int> sortedCorrect = q.correctIndices;
        std::sort(sortedAns.begin(), sortedAns.end());
        std::sort(sortedCorrect.begin(), sortedCorrect.end());
        if (sortedAns == sortedCorrect) ++correct;
    }
    return correct;
}

std::vector<QString> Test::getMistakes() const {
    std::vector<QString> mistakes;
    for (size_t i = 0; i < questions.size(); ++i) {
        const auto& q = questions[i];
        const auto& ans = userAnswers[i];
        std::vector<int> sortedAns = ans;
        std::vector<int> sortedCorrect = q.correctIndices;
        std::sort(sortedAns.begin(), sortedAns.end());
        std::sort(sortedCorrect.begin(), sortedCorrect.end());
        if (sortedAns != sortedCorrect) {
            QString mistake = QString("Вопрос %1: «%2»\n   Ваш ответ: ").arg(i+1).arg(q.text);
            if (ans.empty()) mistake += "нет ответа";
            else {
                for (size_t j = 0; j < ans.size(); ++j) {
                    mistake += q.options[ans[j]];
                    if (j + 1 < ans.size()) mistake += ", ";
                }
            }
            mistake += "\n   Правильный: ";
            for (size_t j = 0; j < q.correctIndices.size(); ++j) {
                mistake += q.options[q.correctIndices[j]];
                if (j + 1 < q.correctIndices.size()) mistake += ", ";
            }
            mistakes.push_back(mistake);
        }
    }
    return mistakes;
}

StartForm::StartForm(Test& test, QWidget* parent) : QWidget(parent), m_test(test) {
    setWindowTitle("Тестирование – Вход");
    setFixedSize(300, 150);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel("Введите ваше имя:");
    label->setStyleSheet("font-size: 14px;");
    layout->addWidget(label);

    nameEdit = new QLineEdit;
    nameEdit->setStyleSheet("font-size: 14px; padding: 5px; border: 1px solid #ccc; border-radius: 3px;");
    layout->addWidget(nameEdit);

    QPushButton* startButton = new QPushButton("Начать тест");
    startButton->setStyleSheet(
        "font-size: 16px; font-weight: bold; "
        "background: #f08a41; color: white; "
        "border-radius: 5px; padding: 8px;"
    );
    layout->addWidget(startButton);

    connect(startButton, &QPushButton::clicked, this, &StartForm::onStart);
}

void StartForm::onStart() {
    QString name = nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите имя!");
        return;
    }
    m_test.setUserName(name);
    emit startTest();
    close();
}

QuestionForm::QuestionForm(Test& test, QWidget* parent)
    : QWidget(parent), m_test(test), m_currentIndex(0), m_timeLeft(30)
{
    setWindowTitle("Тестирование – Вопросы");
    setFixedSize(500, 400);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* topLayout = new QHBoxLayout;
    questionLabel = new QLabel("Вопрос 1 из ...");
    questionLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    topLayout->addWidget(questionLabel);
    topLayout->addStretch();
    timerLabel = new QLabel("Осталось: 30 с");
    timerLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #ff6b6b;");
    topLayout->addWidget(timerLabel);
    mainLayout->addLayout(topLayout);

    questionText = new QLabel;
    questionText->setWordWrap(true);
    questionText->setStyleSheet("font-size: 18px; padding: 10px; background: #f0f0f0; border-radius: 5px;");
    mainLayout->addWidget(questionText);

    optionsContainer = new QWidget;
    optionsLayout = new QVBoxLayout(optionsContainer);
    mainLayout->addWidget(optionsContainer);

    QPushButton* nextButton = new QPushButton("Далее");
    nextButton->setStyleSheet(
        "font-size: 16px; font-weight: bold; "
        "background: #f08a41; color: white; "
        "border-radius: 5px; padding: 8px;"
    );
    mainLayout->addWidget(nextButton);
    connect(nextButton, &QPushButton::clicked, this, &QuestionForm::onNext);

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &QuestionForm::updateTimer);

    showQuestion(0);
}

void QuestionForm::startTimer() {
    m_timeLeft = 30;
    timerLabel->setText(QString("Осталось: %1 с").arg(m_timeLeft));
    timer->start();
}

void QuestionForm::updateTimer() {
    m_timeLeft--;
    timerLabel->setText(QString("Осталось: %1 с").arg(m_timeLeft));
    if (m_timeLeft <= 0) {
        timer->stop();
        QMessageBox::information(this, "Время вышло", "Время на тест истекло!");
        saveCurrentAnswer();
        emit finished();
        close();
    }
}

void QuestionForm::onNext() {
    saveCurrentAnswer();
    m_currentIndex++;
    if (m_currentIndex < m_test.questionCount()) {
        showQuestion(m_currentIndex);
    } else {
        timer->stop();
        emit finished();
        close();
    }
}

void QuestionForm::showQuestion(int index) {
    const Question& q = m_test.getQuestion(index);
    questionLabel->setText(QString("Вопрос %1 из %2").arg(index+1).arg(m_test.questionCount()));
    questionText->setText(q.text);

    clearOptions();

    QString optionStyle = "font-size: 14px; padding: 5px; margin: 2px;";

    if (q.isMultiple) {
        for (size_t i = 0; i < q.options.size(); ++i) {
            QCheckBox* cb = new QCheckBox(q.options[i]);
            cb->setStyleSheet(optionStyle);
            optionsLayout->addWidget(cb);
            m_optionWidgets.push_back(cb);
        }
    } else {
        QButtonGroup* group = new QButtonGroup(this);
        for (size_t i = 0; i < q.options.size(); ++i) {
            QRadioButton* rb = new QRadioButton(q.options[i]);
            rb->setStyleSheet(optionStyle);
            optionsLayout->addWidget(rb);
            group->addButton(rb);
            m_optionWidgets.push_back(rb);
        }
        m_buttonGroups.push_back(group);
    }
}

void QuestionForm::clearOptions() {
    QLayoutItem* item;
    while ((item = optionsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    m_optionWidgets.clear();
    m_buttonGroups.clear();
}

void QuestionForm::saveCurrentAnswer() {
    if (m_currentIndex >= m_test.questionCount()) return;
    const Question& q = m_test.getQuestion(m_currentIndex);
    std::vector<int> selected;
    for (int i = 0; i < (int)m_optionWidgets.size(); ++i) {
        if (q.isMultiple) {
            QCheckBox* cb = qobject_cast<QCheckBox*>(m_optionWidgets[i]);
            if (cb && cb->isChecked()) selected.push_back(i);
        } else {
            QRadioButton* rb = qobject_cast<QRadioButton*>(m_optionWidgets[i]);
            if (rb && rb->isChecked()) selected.push_back(i);
        }
    }
    m_test.setAnswer(m_currentIndex, selected);
}

ResultForm::ResultForm(Test& test, QWidget* parent) : QWidget(parent) {
    setWindowTitle("Результаты тестирования");
    setFixedSize(500, 400);

    QVBoxLayout* layout = new QVBoxLayout(this);

    int score = test.calculateScore();
    int total = test.questionCount();
    QLabel* summary = new QLabel(QString("%1, вы ответили правильно на %2 из %3 вопросов.")
                                      .arg(test.getUserName()).arg(score).arg(total));
    summary->setWordWrap(true);
    summary->setStyleSheet("font-size: 16px; font-weight: bold; padding: 10px; background: #f8f9fa; border-radius: 5px;");
    layout->addWidget(summary);

    auto mistakes = test.getMistakes();
    if (mistakes.empty()) {
        QLabel* perfect = new QLabel("Отлично!) Нет ошибок.");
        perfect->setStyleSheet("font-size: 14px; color: green;");
        layout->addWidget(perfect);
    } else {
        QLabel* errTitle = new QLabel("Допущены ошибки в следующих вопросах:");
        errTitle->setStyleSheet("font-size: 14px; font-weight: bold; margin-top: 10px;");
        layout->addWidget(errTitle);
        for (const auto& m : mistakes) {
            QLabel* err = new QLabel(m);
            err->setWordWrap(true);
            err->setStyleSheet("font-size: 12px; margin-left: 10px; padding: 5px; background: #ffeeee; border-radius: 3px;");
            layout->addWidget(err);
        }
    }

    QPushButton* exitButton = new QPushButton("Выход");
    exitButton->setStyleSheet(
        "font-size: 16px; font-weight: bold; "
        "background: #eb4225; color: white; "
        "border-radius: 5px; padding: 8px;"
    );
    layout->addWidget(exitButton);
    connect(exitButton, &QPushButton::clicked, this, &QWidget::close);
}

TestController::TestController() {
    m_test.addQuestion(Question("Столица Франции?",
                                 {"Париж", "Лондон", "Берлин", "Мадрид"},
                                 {0}));

    m_test.addQuestion(Question("Какие из чисел являются простыми?",
                                 {"2", "4", "7", "9", "13"},
                                 {0,2,4}, true));

    m_test.addQuestion(Question("Солнце — это звезда?",
                                 {"Да", "Нет"},
                                 {0}));

    showStartForm();
}

void TestController::showStartForm() {
    startForm = new StartForm(m_test);
    connect(startForm, &StartForm::startTest, this, &TestController::showQuestionForm);
    startForm->show();
}

void TestController::showQuestionForm() {
    questionForm = new QuestionForm(m_test);
    connect(questionForm, &QuestionForm::finished, this, &TestController::showResultForm);
    questionForm->show();
    questionForm->startTimer();
}

void TestController::showResultForm() {
    resultForm = new ResultForm(m_test);
    resultForm->show();
    if (startForm) startForm->close();
    if (questionForm) questionForm->close();
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    TestController controller;
    return app.exec();
}