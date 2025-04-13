#pragma once
#ifndef MAINWIDGET_H
#define MAINWIDGET_H
#include <QStackedWidget>
#include <QString>

#include "introwidget.h"
#include "rulewidget.h"
#include "managementwidget.h"

class MainWidget final : public QStackedWidget{
    Q_OBJECT
    public:
        explicit MainWidget(QWidget* parent = nullptr);
        ~MainWidget() override;
    private:
        IntroWidget* intro;
        RuleWidget* rule;
        ManagementWidget* questionManagement;

        //  Game config variables
        QString appTitle;
        QString gameTitle;
        bool isMuted;
        int displayQuantity;
        int64_t hardModeCountdownMS;

        int currentGameMode;

        static int getQuestionJsonSize();

    private slots:
        void startGame(int currentMode, bool isMuted);
        void outroCall(int gameMode, int correctCount, int totalCount, bool currentMuted, const std::vector<int64_t>& timestamps);
}; 

#endif