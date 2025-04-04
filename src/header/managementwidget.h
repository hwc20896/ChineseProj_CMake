#pragma once
#ifndef MANAGEMENTWIDGET_H
#define MANAGEMENTWIDGET_H
#include "template.h"
#include "ui_managementwidget.h"
#include <chrono>
#include <vector>
#include <QAudioOutput>
#include <QMediaPlayer>
using namespace std::chrono;

class ManagementWidget final : public QWidget {
    Q_OBJECT
    public:
        explicit ManagementWidget(bool currentMuted, QWidget *parent = nullptr);
        ~ManagementWidget() override;
    private:
        Ui::ManagementWidget* m_ui;
        std::vector<QuestionWidget*> pageWidget;
        std::vector<QuestionData> data;
        static std::vector<QuestionData> getRandomOrder(std::vector<QuestionData> questions, int64_t quantity);

        //  Parse Json
        static std::vector<QuestionData> deserializeJson();

        //  Mute switch related
        QIcon muted, unmuted;
        bool isMuted;

        //  Timer
        high_resolution_clock clock;
        time_point<high_resolution_clock> start, end;
        std::vector<int64_t> timeStamps;

        //  BGM
        QMediaPlayer* player;
        QAudioOutput* output;
    signals:
        void finish();
};

#endif
