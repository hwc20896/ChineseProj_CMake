#include "introwidget.h"

IntroWidget::IntroWidget(QWidget* parent) : QWidget(parent), ui(new Ui::IntroForm){
    ui->setupUi(this);
    isMuted = false;

    //  Connections
    connect(ui->rule, &QPushButton::clicked, this, &IntroWidget::toRulePage);
    connect(ui->startGame, &QPushButton::clicked, this, &IntroWidget::startGame);

    //  Mute switch icons
    muted.addFile(":/Drawables/drawables/mute.png");
    unmuted.addFile(":/Drawables/drawables/unmute.png");
    connect(ui->muteSwitch, &QPushButton::clicked, this, [this] {
        isMuted = !isMuted;
        ui->muteSwitch->setIcon(isMuted ? muted : unmuted);
    });
}

IntroWidget::~IntroWidget(){delete ui;}

void IntroWidget::disable() const {
    ui->chooser->hide();
    ui->explanation->hide();
    ui->startGame->setEnabled(false);
    ui->startGame->setToolTip("由於文件缺失，無法開始游戲");
}