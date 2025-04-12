#include "outrowidget.h"
#include <QApplication>
#include "template.h"

OutroWidget::OutroWidget(QWidget* parent) : QWidget(parent), ui(new Ui::OutroWidget) {
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

    //  Replay
    connect(ui->replayButton, &QPushButton::clicked, this, [this] {
        emit replay(ui->featureBox->currentIndex(), isMuted);
    });

    //  Styles
    ui->replayButton->setObjectName("navigator");
    ui->exitButton->setObjectName("navigator");
    ui->muteSwitch->setObjectName("icon");
    this->setStyleSheet(QuestionWidget::getStyleFromURI(":/Stylesheets/src/css/regular.css"));
}

OutroWidget::~OutroWidget() {delete ui;}

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

void OutroWidget::setMuteSwitchIcon(const bool isMuted) const {
    ui->muteSwitch->setIcon(isMuted ? unmuted : muted);
}

void OutroWidget::setGameMode(const int currentGameMode, const QString& tick) const {
    const QStringList gameModes = {"普通","限時"},
                      modeExplanation = {"普通模式：不限時，結尾顯示答題時間及平均每題時間", QString("限時模式：限時%1，超時將強制跳轉至結束頁").arg(tick)};
    ui->featureBox->addItems(gameModes);
    ui->featureBox->setCurrentIndex(currentGameMode);
    connect(ui->featureBox, &QComboBox::currentIndexChanged, this, [this, modeExplanation] (const int index) {
        ui->gamemodeExplanation->setText(modeExplanation[index]);
    });
    ui->gamemodeExplanation->setText(modeExplanation[currentGameMode]);
}
