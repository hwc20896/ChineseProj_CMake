#pragma once
#ifndef RULEWIDGET_H
#define RULEWIDGET_H
#include "ui_rulewidget.h"

class RuleWidget final : public QWidget{
    Q_OBJECT
    public:
        explicit RuleWidget(QWidget* parent = nullptr);
        ~RuleWidget() override;
    private:
        Ui::RuleForm* ui;
    signals:
        void toIntroPage();
};

#endif