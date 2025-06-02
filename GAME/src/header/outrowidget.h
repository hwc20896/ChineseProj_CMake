#pragma once
#ifndef OUTROWIDGET_H
#define OUTROWIDGET_H
#include "ui_outrowidget.h"
#include <QIcon>
#include "gamemodechooser.h"

class OutroWidget final : public QWidget {
    Q_OBJECT
    public:
        explicit OutroWidget(const QString& tick, int gamemode, QWidget* parent = nullptr);
        ~OutroWidget() override;

        void setTimeDisplay(const QString& totalTime, const QString& avgTime) const;
        void hideTime() const;
        void setScore(const QString& correctText, int totalCount, double percentage) const;
        void setMuteSwitchIcon(bool isMuted);
    private:
        Ui::OutroWidget* ui;

        GamemodeChooser* _chooser;

        //  Mute switch
        QIcon muted, unmuted;
        bool isMuted;
    signals:
        void replay(int currentMode, bool isMuted);
};

#endif