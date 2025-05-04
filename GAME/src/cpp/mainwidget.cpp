#include "mainwidget.h"
#include "outrowidget.h"
#include <QFile>
#include <numeric>

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

    //  SQL parsing
    bool configParseSuccess = false;

    m_query.exec("SELECT * FROM AppConfig");
    if (m_query.next()) {
        configParseSuccess = true;

        this->appTitle  = m_query.value("AppTitle").toString();
        this->gameTitle = m_query.value("GameTitle").toString();
        this->isMuted   = m_query.value("DefaultBGMMute").toBool();
        this->displayQuantity = m_query.value("DisplayQuantity").toInt();
        this->hardModeCountdownMS = m_query.value("HardmodeCountdown").toUInt();

        this->setWindowTitle(this->appTitle);
        intro->enable(appTitle, ManagementWidget::timeDisplay(hardModeCountdownMS));

        rule->setQuantity(getQuestionSize(), this->displayQuantity);
    }

    m_query.exec("SELECT * FROM QuestionData");
    if (!configParseSuccess || !m_query.next()) intro->disable();
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

int MainWidget::getQuestionSize(){
    m_query.exec("SELECT COUNT(ID) FROM QuestionData");
    m_query.next();
    return m_query.value(0).toInt();
}
