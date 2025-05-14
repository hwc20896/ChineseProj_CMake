#pragma once
#ifndef TEMPLATE_H
#define TEMPLATE_H
#include "ui_questionwidget.h"
#include <QJsonObject>
#include <QSoundEffect>
#include <map>
#include <utility>

#define COLOR(target,color) "<font color="#color">"#target"</font>"

struct QuestionData{
    QString title;
    QStringList options;
    int corrOption;

    //  Optional

    QString description;
    QString hint;
    /*   Difficulty:
        1 --> EASY      (general knowledge)
        2 --> MEDIUM    (requires thinking)
        3 --> HARD      (requires a lot of Google)
        4 --> HELL      (almost no one know this, hint or luck required)
    */
    int difficulty;

    explicit QuestionData(QString title, QStringList options, int corroption, QString description = {}, QString hint = {}, const int difficulty = 0)
        : title(std::move(title)), options(std::move(options)), corrOption(corroption), description(std::move(description)), hint(std::move(hint)), difficulty(difficulty){}
};

class QuestionWidget final : public QWidget{
    Q_OBJECT
    public:
        explicit QuestionWidget(const QuestionData& question, int index, QWidget* parent = nullptr);
        ~QuestionWidget() override;

        static QString getStyleFromURI(const QString& uri);
    private:
        QuestionData m_question;
        Ui::QuestionWidget* m_ui;

        //  Contexts
        bool answered;
        QString correctText;
        std::map<QString, QPushButton*> textToButton;
        QString getDifficultyText() const;

        //  Sound Effects
        QSoundEffect* correctSound,* incorrectSound;
        static void cooldown(int msec);
    private slots:
        void answerButtonClicked(QPushButton* targetButton);
    signals:
        void timeTap();
        void score(bool isCorrect);
};


#endif