#include "mainwidget.h"
#include "outrowidget.h"
#include <QFile>
#include <numeric>

MainWidget::MainWidget(const QSqlDatabase& database, QWidget* parent) : QStackedWidget(parent), m_database(database){
    m_query = QSqlQuery(m_database);

    //  SQL parsing
    bool configParseSuccess = false;
    isMuted = false;

    m_query.exec("SELECT * FROM AppConfig");
    if (m_query.next()) {
        configParseSuccess = true;

        this->appTitle  = m_query.value("AppTitle").toString();
        this->gameTitle = m_query.value("GameTitle").toString();
        this->isMuted   = m_query.value("DefaultBGMMute").toBool();
        this->displayQuantity = m_query.value("DisplayQuantity").toInt();
        this->hardModeCountdownMS = m_query.value("HardmodeCountdown").toUInt();

        this->setWindowTitle(this->appTitle);
    }

    intro = new IntroWidget(ManagementWidget::timeDisplay(hardModeCountdownMS));
    rule  = new RuleWidget;

    m_query.exec("SELECT * FROM QuestionData");
    if (!configParseSuccess || !m_query.next()) {
        intro->disable();
    }
    else {
        rule->setQuantity(getQuestionSize(), displayQuantity);
        intro->enable(isMuted, appTitle);
    }

    this->addWidget(intro);
    this->addWidget(rule);
    this->setCurrentWidget(intro);
    this->currentGameMode = 0;

    questionManagement = nullptr;

    //  Connections
    connect(intro, &IntroWidget::toRulePage, this, [this]{this->setCurrentWidget(rule);});
    connect(rule, &RuleWidget::toIntroPage, this, [this]{this->setCurrentWidget(intro);});
    connect(intro, &IntroWidget::startGame, this, &MainWidget::startGame);
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
    questionManagement = new ManagementWidget(m_database, currentMode, isMuted);
    this->currentGameMode = currentMode;
    this->close();
    questionManagement->setWindowTitle(appTitle);
    questionManagement->show();
    questionManagement->resize(this->size());
    connect(questionManagement, &ManagementWidget::finish, this, &MainWidget::outroCall);
}

void MainWidget::outroCall(const int gameMode, const int correctCount, const int totalCount, const bool currentMuted, const std::vector<int64_t>& timestamps) {
    const auto outro = new OutroWidget(ManagementWidget::timeDisplay(hardModeCountdownMS), gameMode);
    outro->setMuteSwitchIcon(currentMuted);
    outro->resize(questionManagement->size());

    //  Score
    outro->setScore(ManagementWidget::addColor(correctCount, totalCount), totalCount, static_cast<double>(correctCount)/static_cast<double>(totalCount)*100);

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
        questionManagement = new ManagementWidget(this->m_database, currentMode, isMuted);
        this->currentGameMode = currentMode;
        outro->close();
        questionManagement->setWindowTitle(appTitle);
        questionManagement->show();
        questionManagement->resize(outro->size());
        connect(questionManagement, &ManagementWidget::finish, this, &MainWidget::outroCall);
    });

    delete questionManagement;
    questionManagement = nullptr;

    this->close();
    outro->show();
}

int MainWidget::getQuestionSize(){
    m_query.exec("SELECT COUNT(*) FROM QuestionData");
    m_query.next();
    return m_query.value(0).toInt();
}
