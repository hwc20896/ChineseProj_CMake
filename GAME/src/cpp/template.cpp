#include "template.h"

#include <QEventLoop>
#include <QFile>
#include <QSoundEffect>
#include <QStyle>
#include <QTimer>
#include <random>
#include <ranges>
#include <QDebug>

#define REFRESH(button)\
this->style()->unpolish(button);\
this->style()->polish(button)

#define SET_PROPERTY(button, value)\
button->setProperty("answer_status",value);\
REFRESH(button)

#define SET_CHOSEN(button) SET_PROPERTY(button,"chosen")

#define SET_CORRECT(button) SET_PROPERTY(button,"correct")

#define SET_INCORRECT(button) SET_PROPERTY(button,"incorrect")

QuestionWidget::QuestionWidget(const QuestionData& question, const int index, QWidget* parent) : QWidget(parent), m_question(question), m_ui(new Ui::QuestionWidget){
    m_ui->setupUi(this);
    answered = false;
    correctText = m_question.options[m_question.corrOption];
    std::ranges::shuffle(m_question.options, std::mt19937((std::random_device()())));

    m_ui->questionTitle->setText(QString("%1 %2: %3").arg(getDifficultyText(),QString::number(index),m_question.title));

    const std::array buttons = {m_ui->optionA, m_ui->optionB, m_ui->optionC, m_ui->optionD};
    for (const auto& button : buttons) {
        button->hide();
        button->setObjectName("option");
        button->setProperty("answer_status","unselected");
    }

    for (auto [text, button] : std::views::zip(m_question.options, buttons)) {
        textToButton.insert({text, button});
        button->show();
        button->setText(text);
        connect(button, &QPushButton::clicked, this, [this, button] {answerButtonClicked(button);});
    }

    //  Sound Effects
    correctSound = new QSoundEffect;
    correctSound->setSource({"qrc:/SoundEffects/medias/bingo.wav"});
    incorrectSound = new QSoundEffect;
    incorrectSound->setSource({"qrc:/SoundEffects/medias/ohno.wav"});

    //  Styles
    this->setStyleSheet(getStyleFromURI(":/Stylesheets/src/css/before_choose.css"));
}

QuestionWidget::~QuestionWidget(){delete m_ui;}

void QuestionWidget::answerButtonClicked(QPushButton* targetButton) {
    if (!answered) {
        answered = true;
        emit timeTap();
        this->setStyleSheet(getStyleFromURI(":/Stylesheets/src/css/after_choose.css"));
        SET_CHOSEN(targetButton);
        const bool corr = targetButton->text() == this->correctText;
        cooldown(800);
        if (corr) {
            SET_CORRECT(targetButton);
            correctSound->play();
        }
        else {
            SET_INCORRECT(targetButton);
            incorrectSound->play();
            cooldown(500);
            const auto corrButton = textToButton[correctText];
            SET_CORRECT(corrButton);
        }
        cooldown(700);
        emit score(corr);
    }
}

void QuestionWidget::cooldown(const int msec) {
    QEventLoop lp;
    QTimer::singleShot(msec,&lp,&QEventLoop::quit);
    lp.exec();
}

QString QuestionWidget::getStyleFromURI(const QString& uri) {
    if (QFile file(uri); file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString result = file.readAll();
        file.close();
        return result;
    }
    return "";
}

QString QuestionWidget::getDifficultyText() const {
    switch (m_question.difficulty) {
        case 0: {
            return "[" COLOR(簡單,"#00ee00") "]";
        }
        case 1: {
            return "[" COLOR(中等,"#d1d100") "]";
        }
        case 2: {
            return "[" COLOR(困難,"#ee0000") "]";
        }
        default: {
            return "[未知]";
        }
    }
}




