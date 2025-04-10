#include "introwidget.h"
#include <QStringList>

IntroWidget::IntroWidget(QWidget* parent) : QWidget(parent), ui(new Ui::IntroForm){
    ui->setupUi(this);
    isMuted = false;

    //  Connections
    connect(ui->rule, &QPushButton::clicked, this, &IntroWidget::toRulePage);
    connect(ui->startGame, &QPushButton::clicked, this, [this]{emit this->startGame(ui->chooser->currentIndex(), isMuted);});

    //  Mute switch icons
    muted.addFile(":/Drawables/drawables/mute.png");
    unmuted.addFile(":/Drawables/drawables/unmute.png");
    connect(ui->muteSwitch, &QPushButton::clicked, this, [this] {
        isMuted = !isMuted;
        ui->muteSwitch->setIcon(isMuted ? muted : unmuted);
    });

    //  Default
    ui->muteSwitch->setIcon(unmuted);
}

IntroWidget::~IntroWidget(){delete ui;}

void IntroWidget::disable() const {
    ui->chooser->hide();
    ui->explanation->hide();
    ui->startGame->setEnabled(false);
    ui->startGame->setToolTip("由於文件缺失，無法開始游戲");
}

void IntroWidget::enable(const QString& appTitle, const QString& tick) const {
    ui->gameTitle->setText(appTitle);
    const QStringList gameMode = {"普通","限時"},
                      modeExplanation = {"普通模式：不限時，結尾顯示答題時間及平均每題時間", QString("限時模式：限時%1，超時將強制跳轉至結束頁").arg(tick)};
    ui->chooser->addItems(gameMode);
    ui->chooser->setCurrentIndex(0);
    connect(ui->chooser, &QComboBox::currentIndexChanged, this, [this, modeExplanation](const int index) {
        ui->explanation->setText(modeExplanation[index]);
    });
    ui->explanation->setText(modeExplanation[0]);
}