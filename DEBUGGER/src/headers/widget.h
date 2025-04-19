#pragma once
#ifndef WIDGET_H
#define WIDGET_H
#include <QSqlQuery>
#include "ui_main.h"

class Widget final : public QWidget {
    Q_OBJECT
    public:
        explicit Widget(const QSqlDatabase& database, QWidget* parent = nullptr);
        ~Widget() override;
    private:
        Ui::mainForm* ui;
};

#endif