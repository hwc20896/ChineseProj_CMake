#include "managementwidget.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <algorithm>
#include <random>
#include <ranges>

ManagementWidget::ManagementWidget(const int mode, const bool currentMuted, QWidget* parent) : QWidget(parent), m_ui(new Ui::ManagementWidget) {
    data = deserializeJson();
    assert(data.size() > 0);
    isMuted = currentMuted;
    isHardMode = mode == 1;
    correctCount = incorrectCount = 0;
    m_ui->setupUi(this);

    //  BGM
    output = new QAudioOutput();
    output->setVolume(.15);
    player = new QMediaPlayer();
    player->setAudioOutput(output);
    player->setSource({"qrc:/BGM/medias/OMFG_Pizza.mp3"});
    connect(player, &QMediaPlayer::mediaStatus, this, [this](const QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::LoadedMedia) player->play();
    });

    //  Json file Read
    if (QFile configFile("questionconfig.json"); configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const auto gameConfig = QJsonDocument::fromJson(configFile.readAll());
        configFile.close();
        assert(gameConfig.isObject());
        const auto config = gameConfig.object();
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
            if (!isHardMode) {
                end = clock.now();
                timeStamps.push_back(duration_cast<milliseconds>(end - start).count());
            }
        });

        //  Score recorder
        connect(widget, &QuestionWidget::score, this, [this](const bool isCorrect) {
            isCorrect? correctCount++ : incorrectCount++;
            m_ui->nextPage->setEnabled(true);
        });
    }

    //  Previous Page button and Next Page button change
    connect(m_ui->stackedWidget, &QStackedWidget::currentChanged, this, [this](const int index) {
        if (index == 0) m_ui->prevPage->hide();
        if (index == displayQuantity - 1) m_ui->nextPage->setText("完成");
    });

    //  Navigations
    connect(m_ui->prevPage, &QPushButton::clicked, this, [this] {m_ui->stackedWidget->setCurrentIndex(m_ui->stackedWidget->currentIndex() - 1);});
    connect(m_ui->nextPage, &QPushButton::clicked, this, [this] {
        if (m_ui->stackedWidget->currentIndex() < displayQuantity -1) {
            m_ui->stackedWidget->setCurrentIndex(m_ui->stackedWidget->currentIndex() + 1);
            if (!isHardMode) start = clock.now();
        }
        else emit finish();
    });

    //  Mute switch alternation
    muted.addFile(":/Drawables/drawables/mute.png");
    unmuted.addFile(":/Drawables/drawables/unmute.png");
    connect(m_ui->muteSwitch, &QPushButton::clicked, this, [this] {
        isMuted = !isMuted;
        output->setMuted(isMuted);
        m_ui->muteSwitch->setIcon(isMuted? muted: unmuted);
    });
}

ManagementWidget::~ManagementWidget() { delete m_ui; }

std::vector<QuestionData> ManagementWidget::getRandomOrder(std::vector<QuestionData> questions, int64_t quantity) {
    std::ranges::shuffle(questions, std::mt19937(std::random_device()()));
    return {questions.begin(), questions.begin() + quantity};
}

std::vector<QuestionData> ManagementWidget::deserializeJson() {
    std::vector<QuestionData> result;
    if (QFile questionDataFile("questionlist.json"); questionDataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const auto doc = QJsonDocument::fromJson(questionDataFile.readAll());
        questionDataFile.close();
        assert(doc.isArray());
        std::ranges::for_each(doc.array(), [&result](const QJsonValue& value) {
            result.emplace_back(value.toObject());
        });
    }
    return result;
}


