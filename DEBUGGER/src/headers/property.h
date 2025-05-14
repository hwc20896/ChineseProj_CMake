#pragma once
#ifndef PROPERTY_H
#define PROPERTY_H
#include <QSqlQuery>
#include "ui_property.h"

class Property final : public QWidget{
    public:
        explicit Property(const QSqlDatabase& database, QWidget* parent = nullptr);
        ~Property() override;
    private:
        QSqlQuery m_query;
        Ui::Property* ui;
};

#endif