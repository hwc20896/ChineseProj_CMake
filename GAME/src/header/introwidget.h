#pragma once
#ifndef INTROWIDGET_H
#define INTROWIDGET_H

#include "ui_introwidget.h"
#include <QIcon>
#include "gamemodechooser.h"

class IntroWidget final : public QWidget{
    Q_OBJECT
    public:
        explicit IntroWidget(const QString& tick, QWidget* parent = nullptr);
        ~IntroWidget() override;
        void disable() const;
        void enable(bool isMuted, const QString& appTitle);
    private:
        Ui::IntroForm* ui;

        //  Icons
        QIcon muted, unmuted;
        bool isMuted;
        GamemodeChooser* _chooser;
    signals:
        void toRulePage();
        void startGame(int currentMode, bool isMuted);
};

#endif