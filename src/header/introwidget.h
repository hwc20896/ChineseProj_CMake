#pragma once
#ifndef INTROWIDGET_H
#define INTROWIDGET_H

#include "ui_introwidget.h"
#include <QIcon>

class IntroWidget final : public QWidget{
    Q_OBJECT
    public:
        explicit IntroWidget(QWidget* parent = nullptr);
        ~IntroWidget() override;
        void disable() const;
    private:
        Ui::IntroForm* ui;

        //  Icons
        QIcon muted, unmuted;
        bool isMuted;
    signals:
        void toRulePage();
        void startGame(bool isMuted);
};

#endif