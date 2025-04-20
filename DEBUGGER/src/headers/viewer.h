#pragma once
#ifndef VIEWER_H
#define VIEWER_H
#include "ui_questionview.h"
#include <QSqlQuery>

class Viewer final : public QWidget {
    Q_OBJECT
    public:
        explicit Viewer(const QString& target, const QSqlDatabase& database, QWidget* parent = nullptr);
        ~Viewer() override;
    private:
        QSqlQuery m_query;

        Ui::QuestionViewForm* ui;
};

#endif