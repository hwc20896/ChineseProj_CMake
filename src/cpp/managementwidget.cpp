#include "managementwidget.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <algorithm>
#include <random>
#include <ranges>

ManagementWidget::ManagementWidget(const int mode, const bool currentMuted, QWidget* parent) : QWidget(parent), m_ui(new Ui::ManagementWidget) {
    data = deserializeJson();
    assert(!data.empty());
    isMuted = currentMuted;
    currentGameMode = mode;
    correctCount = incorrectCount = 0;
    hasAnswered = std::vector(data.size(), false);
    m_ui->setupUi(this);

    m_ui->stackedWidget->removeWidget(m_ui->page);
    m_ui->stackedWidget->removeWidget(m_ui->page_2);

    //  BGM
    output = new QAudioOutput();
    output->setVolume(.15);
    player = new QMediaPlayer();
    player->setAudioOutput(output);
    player->setSource({"qrc:/BGM/medias/OMFG_Pizza.mp3"});
    connect(player, &QMediaPlayer::mediaStatusChanged, this, [this](const QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::LoadedMedia) player->play();
    });

    //  Json file Read
    if (QFile configFile("questionconfig.json"); configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const auto gameConfig = QJsonValue::fromJson(configFile.readAll());
        configFile.close();
        assert(gameConfig.isObject());
        const auto config = gameConfig.toObject();
        countdownTime = config["hardmode_countdown_ms"].toInt();
        displayQuantity = config["display_quantity"].toInt();
    }

    //  Question data insert to widget
    for (const auto indexes = std::views::iota(0, displayQuantity);
         auto [currentIndex, currentData] : std::ranges::zip_view(indexes, getRandomOrder(data, displayQuantity))
    ) {
        const auto widget = new QuestionWidget(currentData, currentIndex+1);
        m_ui->stackedWidget->addWidget(widget);

        //  Time recorder for total time and time per question use
        connect(widget, &QuestionWidget::timeTap, this, [this] {
            if (currentGameMode != 1) {
                end = high_resolution_clock::now();
                timeStamps.push_back(duration_cast<milliseconds>(end - start).count());
            }
        });

        //  Score recorder
        connect(widget, &QuestionWidget::score, this, [this, currentIndex](const bool isCorrect) {
            isCorrect? correctCount++ : incorrectCount++;
            m_ui->nextPage->setEnabled(true);
            hasAnswered[currentIndex] = true;
        });
    }

    //  Default
    m_ui->stackedWidget->setCurrentIndex(0);

    //  Enable timer in Hard Mode
    m_ui->timeDisplay->setVisible(currentGameMode == 1);

    //  Previous Page button and Next Page button change
    connect(m_ui->stackedWidget, &QStackedWidget::currentChanged, this, [this](const int index) {
        m_ui->prevPage->setVisible(index != 0);
        m_ui->nextPage->setText(index < displayQuantity - 1? "下一頁 →": "完成");
        m_ui->nextPage->setEnabled(hasAnswered[index]);
    });
    m_ui->prevPage->hide();

    //  Navigations
    connect(m_ui->prevPage, &QPushButton::clicked, this, [this] {m_ui->stackedWidget->setCurrentIndex(m_ui->stackedWidget->currentIndex() - 1);});
    connect(m_ui->nextPage, &QPushButton::clicked, this, [this] {
        if (m_ui->stackedWidget->currentIndex() < displayQuantity -1) {
            m_ui->stackedWidget->setCurrentIndex(m_ui->stackedWidget->currentIndex() + 1);
            if (currentGameMode != 1) start = high_resolution_clock::now();
        }
        else emit finish(this->timeStamps, currentGameMode, correctCount, displayQuantity);
    });

    //  Mute switch alternation
    muted.addFile(":/Drawables/drawables/mute.png");
    unmuted.addFile(":/Drawables/drawables/unmute.png");
    connect(m_ui->muteSwitch, &QPushButton::clicked, this, [this] {
        isMuted = !isMuted;
        output->setMuted(isMuted);
        m_ui->muteSwitch->setIcon(isMuted? muted: unmuted);
    });
    m_ui->muteSwitch->setIcon(currentMuted? muted: unmuted);
    output->setMuted(currentMuted);

    start = high_resolution_clock::now();

    //  Styles
    m_ui->muteSwitch->setObjectName("icon");
    m_ui->nextPage->setObjectName("navigator");
    m_ui->prevPage->setObjectName("navigator");
    this->setStyleSheet(QuestionWidget::getStyleFromURI(":/Stylesheets/src/css/regular.css"));
}

ManagementWidget::~ManagementWidget() {
    delete m_ui;
    player->stop();
}

std::vector<QuestionData> ManagementWidget::getRandomOrder(std::vector<QuestionData> questions, int64_t quantity) {
    std::ranges::shuffle(questions, std::mt19937(std::random_device()()));
    return {questions.begin(), questions.begin() + quantity};
}

std::vector<QuestionData> ManagementWidget::deserializeJson() {
    std::vector<QuestionData> result;
    if (QFile questionDataFile("questionlist.json"); questionDataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const auto doc = QJsonValue::fromJson(questionDataFile.readAll());
        questionDataFile.close();
        assert(doc.isArray());
        for (const auto& item : doc.toArray()) {
            result.emplace_back(item.toObject());
        }
    }
    return result;
}

QString ManagementWidget::timeDisplay(const double duration) {
    if (duration > 60000LL) {
        const double remain = remainder(duration,60.0);
        return QString("%1分%2秒").arg(QString::number(std::floor(duration/60000LL)), QString::number(remain<0?remain+60:remain,'g',3));
    }
    if (duration == 60000LL) return "1分鐘";
    if (duration >= 0) return QString("%1秒").arg(QString::number(duration/1000.0,'g',3));
    throw std::range_error("Invalid duration");
}

QString ManagementWidget::addColor(const int correctCount, const int total) {
    const double rate = static_cast<double>(correctCount) / static_cast<double>(total) * 100;
    /*  (80% ~ 100%] -> S
     *  (55% ~ 80%] -> A
     *  (30% ~ 55%] -> B
     *  [0% ~ 30%] -> C
     */
    if (rate > 80 && rate <= 100) return QString(COLOR(%1,"#e0cf37")).arg(QString::number(correctCount));
    if (rate > 55 && rate <= 80) return QString(COLOR(%1,"#8a43c1")).arg(QString::number(correctCount));
    if (rate > 30 && rate <= 55) return QString(COLOR(%1,"#0ebd2f")).arg(QString::number(correctCount));
    if (rate >= 0 && rate <= 30) return QString(COLOR(%1,"#343bcd")).arg(QString::number(correctCount));
    throw std::range_error("Rate out of range: Pls Check");
}



