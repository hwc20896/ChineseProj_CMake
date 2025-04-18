#pragma once
#ifndef MANAGEMENTWIDGET_H
#define MANAGEMENTWIDGET_H
#include "template.h"
#include "ui_managementwidget.h"
#include <QAudioOutput>
#include <QMediaPlayer>
#include <chrono>
#include <vector>
#include <QTimer>
using namespace std::chrono;

#define COLOR(target,color) "<font color="#color">"#target"</font>"

class ManagementWidget final : public QWidget {
    Q_OBJECT
    public:
        explicit ManagementWidget(int mode, bool currentMuted, QWidget *parent = nullptr);
        ~ManagementWidget() override;

        //  Time display function
        static QString timeDisplay(double duration);

        //  Add color
        static QString addColor(int correctCount, int total);
    private:
        Ui::ManagementWidget* m_ui;
        std::vector<QuestionWidget*> pageWidget;
        std::vector<QuestionData> data;
        static std::vector<QuestionData> getRandomOrder(std::vector<QuestionData> questions, int64_t quantity);
        std::vector<bool> hasAnswered;

        void updatePages() const;
        void setScore(int corr, int inCorr) const;
        void setProgress(int currentProgress, int total) const;

        //  Parse Json
        static std::vector<QuestionData> deserializeJson();

        //  Mute switch related
        QIcon muted, unmuted;
        bool isMuted;

        //  Timer
        time_point<high_resolution_clock> start, end;
        std::vector<int64_t> timeStamps;

        //  BGM
        QMediaPlayer* player;
        QAudioOutput* output;

        //  Game config
        int64_t displayQuantity;
        int currentGameMode;

        //  Hard mode
        int64_t countdownTime, timeLimit;
        QTimer* countdown,* tick;
        void updateTime() const;

        //  Game status
        int correctCount, incorrectCount;
    signals:
        void finish(int currentGameMode, int correctCount, int totalCount, bool currentMuted, const std::vector<int64_t>& timestamps = {});
};

#endif
