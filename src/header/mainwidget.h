#pragma once
#ifndef MAINWIDGET_H
#define MAINWIDGET_H
#include <QStackedWidget>
#include <QString>

#include "introwidget.h"
#include "rulewidget.h"

class MainWidget : public QStackedWidget{
    Q_OBJECT
    public:
        explicit MainWidget(QWidget* parent = nullptr);
        virtual ~MainWidget();
    private:
        IntroWidget* intro;
        RuleWidget* rule;

        //  Game config variables
        QString appTitle;
        QString gameTitle;
        bool isMuted;
    private slots:
        void startGame();
}; 

#endif