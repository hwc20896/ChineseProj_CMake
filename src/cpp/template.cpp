#include "template.h"

#include <ranges>
#include <random>
#include <QSoundEffect>
#include <QEventLoop>
#include <QTimer>
#include <QStyle>
#include <QFile>

#define REFRESH(button)\
this->style()->unpolish(button);\
this->style()->polish(button)

#define SET_PROPERTY(button, value)\
button->setProperty("answer_status",value);\
REFRESH(button)

#define SET_CHOSEN(button) SET_PROPERTY(button,"chosen")

#define SET_CORRECT(button) SET_PROPERTY(button,"correct")

#define SET_INCORRECT(button) SET_PROPERTY(button,"incorrect")

QuestionWidget::QuestionWidget(QuestionData question, int index, QWidget* parent) : QWidget(parent), m_question(std::move(question)), m_ui(new Ui::QuestionWidget){
    m_ui->setupUi();
    answered = false;
    correctText = m_question.options[m_question.corrOption];
    std::ranges::shuffle(m_question.options, std::mt19937((std::random_device()())));

    for (std::initializer_list buttons = {m_ui->optionA, m_ui->optionB, m_ui->optionC, m_ui->optionD};
        auto [text, button] : std::ranges::zip_view(m_question.options,buttons)
    ) {
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
}

QuestionWidget::~QuestionWidget(){delete m_ui;}

void QuestionWidget::answerButtonClicked(QPushButton* targetButton) {
    if (!answered) {
        answered = true;
        emit timeTap();
        SET_CHOSEN(targetButton);
        const bool corr = targetButton->text() == this->correctText;
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



