#include "mainwidget.h"
#include "outrowidget.h"
#include <QFile>
#include <QJsonDocument>
#include <numeric>

MainWidget::MainWidget(QWidget* parent) : QStackedWidget(parent), intro(new IntroWidget), rule(new RuleWidget){
    this->addWidget(intro);
    this->addWidget(rule);
    this->setCurrentWidget(intro);

    questionManagement = nullptr;

    //  Connections
    connect(intro, &IntroWidget::toRulePage, this, [this]{this->setCurrentWidget(rule);});
    connect(rule, &RuleWidget::toIntroPage, this, [this]{this->setCurrentWidget(intro);});
    connect(intro, &IntroWidget::startGame, this, &MainWidget::startGame);

    //  Json parsing
    if (QFile configFile("appconfig.json"); configFile.open(QIODevice::ReadOnly | QIODevice::Text) && QFile::exists("questionlist.json") && QFile::exists("questionconfig.json")) {
        const auto doc = QJsonDocument::fromJson(configFile.readAll());
        configFile.close();
        assert(doc.isObject());
        auto object = doc.object();

        //  Applying json configs
        this->appTitle  = object["app_title"].toString("某游戲");
        this->gameTitle = object["game_title"].toString("游戲標題");
        this->isMuted   = object["default_background_mute"].toBool(true);
        
        this->setWindowTitle(this->appTitle);
    }
    else intro->disable();
}

MainWidget::~MainWidget(){
    delete intro;
    delete rule;
}

/*  @param currentMode
 *      0 -> normal mode
 *      1 -> timed mode
 *  more mode coming soon...
 */
void MainWidget::startGame(const int currentMode, const bool isMuted) {
    questionManagement = new ManagementWidget(currentMode, isMuted);
    this->close();
    questionManagement->show();
    connect(questionManagement, &ManagementWidget::finish, this, &MainWidget::outroCall);
}

void MainWidget::outroCall(const std::vector<int64_t>& timestamps, const int gameMode, const int correctCount, const int totalCount) {
    const auto outro = new OutroWidget;

    //  Score
    outro->setScore(ManagementWidget::addColor(correctCount, totalCount),totalCount, static_cast<double>(correctCount)/static_cast<double>(totalCount)*100);

    //  Timer
    if (gameMode != 1) {
        const double totalTime = std::accumulate(timestamps.begin(), timestamps.end(), 0.0);
        outro->setTimeDisplay(ManagementWidget::timeDisplay(totalTime), ManagementWidget::timeDisplay(totalTime/static_cast<double>(timestamps.size())));
    }
    else {
        outro->hideTime();
    }
    connect(outro, &OutroWidget::replay, this, &MainWidget::startGame);

    delete questionManagement;
    questionManagement = nullptr;

    this->close();
    outro->show();
}