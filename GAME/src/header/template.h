#pragma once
#ifndef TEMPLATE_H
#define TEMPLATE_H
#include "ui_questionwidget.h"
#include <QJsonObject>
#include <QSoundEffect>
#include <map>
#include <utility>

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

    explicit QuestionData(QString  title, QStringList options, int corroption, QString  description = {}, QString  hint = {}, const int difficulty = 0)
        : title(std::move(title)), options(std::move(options)), corrOption(corroption), description(std::move(description)), hint(std::move(hint)), difficulty(difficulty){}
    explicit QuestionData(const QJsonObject& object)
        : title(object["questiontitle"].toString()), options(object["options"].toVariant().toStringList()), corrOption(object["corroption"].toInt()),
          description(object["description"].toString()), hint(object["hint"].toString()), difficulty(object["difficulty"].toInt()){}
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