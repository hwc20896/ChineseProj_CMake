#pragma once
#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QSqlQuery>

class Widget final : public QWidget {
    Q_OBJECT
    public:
        explicit Widget(const QSqlDatabase& database, QWidget* parent = nullptr);
        ~Widget() override;
    private:
        QSqlDatabase m_database;
        QSqlQuery m_query;
};

#endif