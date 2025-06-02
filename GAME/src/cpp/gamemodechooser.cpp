#include "gamemodechooser.h"

GamemodeChooser::GamemodeChooser(const QString& tick, int gamemode, QWidget* parent) : QWidget(parent), _ui(new Ui::GamemodeChooser) {
    _ui->setupUi(this);

    const QStringList gameMode = {
        "普通",
        "限時",
        "簡單"
    },
    modeExplanation = {
        "普通模式：不限時，結尾顯示答題時間及平均每題時間",
        QString("限時模式：限時%1，超時將強制跳轉至結束頁").arg(tick),
        "簡單模式：僅有簡單題目，共有32題"
    };

    _ui->chooser->addItems(gameMode);
    _ui->chooser->setCurrentIndex(0);
    _ui->explanation->setText(modeExplanation[0]);
    connect(_ui->chooser, &QComboBox::currentIndexChanged, this, [this, modeExplanation](const int index) {
        _ui->explanation->setText(modeExplanation[index]);
    });
    _ui->chooser->setCurrentIndex(gamemode);
}

GamemodeChooser::~GamemodeChooser() {
    delete _ui;
}

int GamemodeChooser::getMode() const {
    return _ui->chooser->currentIndex();
}

void GamemodeChooser::disableWidget() const {
    _ui->chooser->setEnabled(false);
    _ui->explanation->hide();
}

void GamemodeChooser::setCurrentMode(const int mode) const {
    _ui->chooser->setCurrentIndex(mode);
}
