#include "outrowidget.h"
#include <QApplication>
#include "template.h"

OutroWidget::OutroWidget(const QString& tick, int gamemode, QWidget* parent) : QWidget(parent), ui(new Ui::OutroWidget) {
    ui->setupUi(this);

    //  Mute switch
    muted.addFile(":/Drawables/drawables/mute.png");
    unmuted.addFile(":/Drawables/drawables/unmute.png");
    connect(ui->muteSwitch, &QPushButton::clicked, this, [this] {
        isMuted = !isMuted;
        ui->muteSwitch->setIcon(isMuted ? muted : unmuted);
    });

    //  Quit
    connect(ui->exitButton, &QPushButton::clicked, this, &QApplication::quit);

    _chooser = new GamemodeChooser(tick, gamemode);
    ui->mainLayout->insertWidget(2, _chooser);

    //  Replay
    connect(ui->replayButton, &QPushButton::clicked, this, [this] {
        emit replay(_chooser->getMode(), isMuted);
    });

    //  Styles
    ui->replayButton->setObjectName("navigator");
    ui->exitButton->setObjectName("navigator");
    ui->muteSwitch->setObjectName("icon");
    this->setStyleSheet(QuestionWidget::getStyleFromURI(":/Stylesheets/src/css/regular.css"));
}

OutroWidget::~OutroWidget() {
    delete ui;
}

void OutroWidget::setTimeDisplay(const QString& totalTime, const QString& avgTime) const {
    ui->totalTime->setText(QString("總答題時間：%1").arg(totalTime));
    ui->avgTime->setText(QString("平均答題時間：%1").arg(avgTime));
}

void OutroWidget::hideTime() const {
    ui->totalTime->hide();
    ui->avgTime->hide();
}
void OutroWidget::setScore(const QString& correctText, const int totalCount, const double percentage) const {
    ui->score->setText(QString("你的得分為：%1 / %2：%3%").arg(correctText, QString::number(totalCount), QString::number(percentage)));
}

void OutroWidget::setMuteSwitchIcon(const bool isMuted) {
    this->isMuted = isMuted;
    ui->muteSwitch->setIcon(isMuted ? muted: unmuted);
}
