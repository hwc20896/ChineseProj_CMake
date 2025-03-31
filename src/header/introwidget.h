#pragma once
#ifndef INTROWIDGET_H
#define INTROWIDGET_H
#include <QWidget>
#include "ui_introwidget.h"

class IntroWidget : public QWidget{
    Q_OBJECT
    public:
        explicit IntroWidget(QWidget* parent = nullptr);
        virtual ~IntroWidget();
    private:
        Ui::IntroForm* ui;
    signals:
        void toRulePage();
};

#endif