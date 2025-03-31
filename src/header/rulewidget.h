#pragma once
#ifndef RULEWIDGET_H
#define RULEWIDGET_H
#include <QWidget>
#include "ui_rulewidget.h"

class RuleWidget : public QWidget{
    Q_OBJECT
    public:
        explicit RuleWidget(QWidget* parent = nullptr);
        virtual ~RuleWidget();
    private:
        Ui::RuleForm* ui;
    signals:
        void toIntroPage();
};

#endif