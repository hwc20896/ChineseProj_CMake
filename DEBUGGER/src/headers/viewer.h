#pragma once
#ifndef VIEWER_H
#define VIEWER_H
#include "ui_questionview.h"
#include <QSqlQuery>
#include <map>

class Viewer final : public QWidget {
    Q_OBJECT
    public:
        explicit Viewer(const QString& target, const QSqlDatabase& database, QWidget* parent = nullptr);
        ~Viewer() override;

        static QString getStyle(const QString& uri);
    private:
        QSqlQuery m_query;
        std::map<int, QPushButton*> index_to_button;
        std::map<int, QString> difficulty_to_color;

        Ui::QuestionViewForm* ui;
};

#endif