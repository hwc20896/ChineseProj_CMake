#pragma once
#ifndef QUESTIONCONTAINER_H
#define QUESTIONCONTAINER_H
#include "ui_questioncontainer.h"
#include <QSqlQuery>

class QuestionContainer final : public QWidget {
    Q_OBJECT
    public:
        explicit QuestionContainer(const QSqlDatabase& database, QWidget* parent = nullptr);
        ~QuestionContainer() override;
    private:
        QSqlQuery m_query;
        Ui::QuestionListForm* ui;
        QSqlDatabase m_database;

        QStringList getQuestionList();
};

#endif