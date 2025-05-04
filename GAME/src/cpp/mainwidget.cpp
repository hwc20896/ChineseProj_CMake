#include "mainwidget.h"
#include "outrowidget.h"
#include <QFile>
#include <QJsonDocument>
#include <numeric>
#include <QJsonArray>

MainWidget::MainWidget(const QSqlDatabase& database, QWidget* parent) : QStackedWidget(parent), intro(new IntroWidget), rule(new RuleWidget), m_database(database){
    m_query = QSqlQuery(m_database);

    this->addWidget(intro);
    this->addWidget(rule);
    this->setCurrentWidget(intro);
    this->currentGameMode = 0;

    questionManagement = nullptr;

    //  Connections
    connect(intro, &IntroWidget::toRulePage, this, [this]{this->setCurrentWidget(rule);});
    connect(rule, &RuleWidget::toIntroPage, this, [this]{this->setCurrentWidget(intro);});
    connect(intro, &IntroWidget::startGame, this, &MainWidget::startGame);

    //  Json parsing
    if (QFile configFile("appconfig.json"); configFile.open(QIODevice::ReadOnly | QIODevice::Text) && QFile::exists("questionlist.json")) {
        const auto doc = QJsonDocument::fromJson(configFile.readAll());
        configFile.close();
        assert(doc.isObject());
        auto object = doc.object();

        //  Applying json configs
        this->appTitle  = object["app_title"].toString("某游戲");
        this->gameTitle = object["game_title"].toString("游戲標題");
        this->isMuted   = object["default_background_mute"].toBool(true);
        this->displayQuantity = object["display_quantity"].toInt();
        this->hardModeCountdownMS = object["hardmode_countdown_ms"].toInt();
        
        this->setWindowTitle(this->appTitle);
        intro->enable(gameTitle, ManagementWidget::timeDisplay(hardModeCountdownMS));

        //  Rule
        rule->setQuantity(getQuestionJsonSize(), displayQuantity);
    }
    else intro->disable();
}

MainWidget::~MainWidget(){
    delete intro;
    delete rule;
    m_database.close();
}

/*  @param currentMode
    0 -> normal mode
    1 -> timed mode
    more mode coming soon...
 */
void MainWidget::startGame(const int currentMode, const bool isMuted) {
    questionManagement = new ManagementWidget(currentMode, isMuted);
    this->currentGameMode = currentMode;
    this->close();
    questionManagement->show();
    questionManagement->resize(this->size());
    connect(questionManagement, &ManagementWidget::finish, this, &MainWidget::outroCall);
}

void MainWidget::outroCall(const int gameMode, const int correctCount, const int totalCount, const bool currentMuted, const std::vector<int64_t>& timestamps) {
    const auto outro = new OutroWidget;
    outro->setMuteSwitchIcon(currentMuted);
    outro->resize(questionManagement->size());

    //  Score
    outro->setScore(ManagementWidget::addColor(correctCount, totalCount), totalCount, static_cast<double>(correctCount)/static_cast<double>(totalCount)*100);
    outro->setGameMode(currentGameMode, ManagementWidget::timeDisplay(hardModeCountdownMS));

    //  Timer
    if (gameMode != 1) {
        const double totalTime = std::accumulate(timestamps.begin(), timestamps.end(), 0.0);
        outro->setTimeDisplay(ManagementWidget::timeDisplay(totalTime), ManagementWidget::timeDisplay(totalTime/static_cast<double>(timestamps.size())));
    }
    else {
        outro->hideTime();
    }
    connect(outro, &OutroWidget::replay, this, [this, outro](const int currentMode, const bool isMuted) {
        //  Replay
        questionManagement = new ManagementWidget(currentMode, isMuted);
        this->currentGameMode = currentMode;
        outro->close();
        questionManagement->show();
        questionManagement->resize(outro->size());
        connect(questionManagement, &ManagementWidget::finish, this, &MainWidget::outroCall);
    });

    delete questionManagement;
    questionManagement = nullptr;

    this->close();
    outro->show();
}

int MainWidget::getQuestionJsonSize() {
    if (QFile questionJson("questionlist.json"); questionJson.open(QIODevice::ReadOnly | QIODevice::Text)){
        const auto data = QJsonValue::fromJson(questionJson.readAll());
        questionJson.close();
        return data.isArray() ? data.toArray().size() : -1;
    }
    throw std::runtime_error("not found");
}
