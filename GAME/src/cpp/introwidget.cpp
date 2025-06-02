#include "introwidget.h"

#include <QStringList>
#include "template.h"

IntroWidget::IntroWidget(const QString& tick, QWidget* parent) : QWidget(parent), ui(new Ui::IntroForm){
    ui->setupUi(this);
    isMuted = false;

    //  Connections
    connect(ui->rule, &QPushButton::clicked, this, &IntroWidget::toRulePage);
    connect(ui->startGame, &QPushButton::clicked, this, [this]{emit this->startGame(_chooser->getMode(), isMuted);});

    //  Mute switch icons
    muted.addFile(":/Drawables/drawables/mute.png");
    unmuted.addFile(":/Drawables/drawables/unmute.png");
    connect(ui->muteSwitch, &QPushButton::clicked, this, [this] {
        isMuted = !isMuted;
        ui->muteSwitch->setIcon(isMuted ? muted : unmuted);
    });

    //  Default
    ui->muteSwitch->setIcon(unmuted);

    //  Chooser
    _chooser = new GamemodeChooser(tick);
    ui->mainLayout->insertWidget(2, _chooser);

    //  Styles
    ui->muteSwitch->setObjectName("icon");
    ui->startGame->setObjectName("navigator");
    ui->rule->setObjectName("navigator");
    this->setStyleSheet(QuestionWidget::getStyleFromURI(":/Stylesheets/src/css/regular.css"));
}

IntroWidget::~IntroWidget(){delete ui;}

void IntroWidget::disable() const {
    ui->startGame->setEnabled(false);
    ui->startGame->setToolTip("由於文件缺失，無法開始游戲");
}

void IntroWidget::enable(const bool isMuted, const QString& appTitle){
    //  Title
    ui->gameTitle->setText(appTitle);

    //  Mute status
    this->isMuted = isMuted;
    ui->muteSwitch->setIcon(isMuted ? muted : unmuted);
}