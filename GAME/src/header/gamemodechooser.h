#pragma once
#ifndef GAMEMODE_CHOOSER_H
#define GAMEMODE_CHOOSER_H
#include "ui_gamemodechooser.h"

class GamemodeChooser final : public QWidget {
    Q_OBJECT
    public:
        explicit GamemodeChooser(const QString& tick, int gamemode = 0, QWidget* parent = nullptr);
        ~GamemodeChooser() override;

        [[nodiscard]]
        int getMode() const;
        void disableWidget() const;
        void setCurrentMode(int mode) const;
    private:
        Ui::GamemodeChooser* _ui;
};

#endif