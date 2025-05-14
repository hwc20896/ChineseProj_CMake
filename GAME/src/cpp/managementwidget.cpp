#include "managementwidget.h"
#include <QFile>
#include <algorithm>
#include <random>
#include <ranges>

#define RANDOM_ALGORITHM std::mt19937(std::random_device()())

ManagementWidget::ManagementWidget(const QSqlDatabase& database, const int mode, const bool currentMuted, QWidget* parent) : QWidget(parent), m_ui(new Ui::ManagementWidget), m_database(database) {
    m_query.exec("SELECT DisplayQuantity FROM AppConfig");
    m_query.next();
    displayQuantity = m_query.value(0).toULongLong();

    data = getSQLQuestions();
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
    player->setLoops(QMediaPlayer::Loops::Infinite);
    player->play();
    connect(qApp, &QApplication::aboutToQuit, player, &QMediaPlayer::stop);

    //  SQL property read
    m_query.exec("SELECT HardmodeCountdown, DisplayQuantity FROM AppConfig");
    m_query.next();
    countdownTime   = m_query.value(0).toULongLong();

    //  Question data insert to widget
    for (const auto indexes = std::views::iota(0, displayQuantity);
         auto [currentIndex, currentData] : std::ranges::zip_view(indexes, data)
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
            updatePages();
        });
    }

    //  Default
    m_ui->stackedWidget->setCurrentIndex(0);
    updatePages();

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
        else emit finish(currentGameMode, correctCount, displayQuantity, isMuted, this->timeStamps);
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

    if (currentGameMode != 1) start = high_resolution_clock::now();
    else {
        timeLimit = countdownTime;
        countdown = new QTimer;
        tick = new QTimer;
        connect(countdown, &QTimer::timeout, this, [this] {emit finish(currentGameMode,correctCount,displayQuantity,isMuted);});
        connect(tick, &QTimer::timeout, this, [this] {
            timeLimit -= 1000;
            updateTime();
        });
        countdown->start(countdownTime);
        tick->start(1000);
        updateTime();
    }

    //  Styles
    m_ui->muteSwitch->setObjectName("icon");
    m_ui->nextPage->setObjectName("navigator");
    m_ui->prevPage->setObjectName("navigator");
    this->setStyleSheet(QuestionWidget::getStyleFromURI(":/Stylesheets/src/css/regular.css"));
}

ManagementWidget::~ManagementWidget() {
    delete m_ui;
    player->disconnect();
    player->stop();
    m_database.close();
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

void ManagementWidget::setScore(const int corr, const int inCorr) const {
    m_ui->correctState->setText(QString(COLOR(錯誤數 %1,"#ff0000")" | " COLOR(%2 正確數,"#00dd12")).arg(QString::number(inCorr), QString::number(corr)));
}

void ManagementWidget::setProgress(const int currentProgress, const int total) const {
    m_ui->progress->setText(QString("進度：%1 / %2 - %3%").arg(QString::number(currentProgress), QString::number(total), QString::number(currentProgress*100/total)));
}

void ManagementWidget::updatePages() const {
    this->setScore(correctCount, incorrectCount);
    this->setProgress(correctCount + incorrectCount, displayQuantity);
}

void ManagementWidget::updateTime() const {
    const int sec = timeLimit / 1000;
    m_ui->timeDisplay->setText(
        QString(sec >= 20 ? "剩餘時間: %1:%2" : "剩餘時間: " COLOR(%1,"#ff0000") " : " COLOR(%2,"#ff0000"))
        .arg(QString::number(std::floor(timeLimit / countdownTime)),
            sec%60 < 10 ? QString("0%1").arg(QString::number(sec%60)) : QString::number(sec%60)
        )
    );
}

std::vector<QuestionData> ManagementWidget::getSQLQuestions() {
    std::vector<QuestionData> output;
    std::vector<int> idPool, sampled(displayQuantity);
    m_query.exec(currentGameMode == 2? "SELECT ID FROM QuestionData WHERE DIFFICULTY = 0": "SELECT ID FROM QuestionData");
    while (m_query.next()) idPool.push_back(m_query.value(0).toInt());
    std::ranges::sample(idPool, sampled.begin(), displayQuantity, RANDOM_ALGORITHM);
    for (const auto& i : sampled) {
        m_query.prepare("SELECT * FROM QuestionData WHERE ID = ?");
        m_query.addBindValue(i);
        m_query.exec();
        m_query.next();

        //  Get question data
        const auto questionTitle = m_query.value("QuestionTitle").toString();
        QStringList options;
        for (const auto& element : {"Option1","Option2","Option3","Option4"}) {
            if (const auto option = m_query.value(element); !option.isNull()) {
                options.push_back(option.toString());
            }
        }
        const auto correctOption = m_query.value("CorrectOption").toInt(),
                   difficulty    = m_query.value("DIFFICULTY").toInt();
        const auto description      = m_query.value("Description").toString(),
                   hint             = m_query.value("Hint").toString();
        output.emplace_back(questionTitle, options, correctOption, description, hint, difficulty);
    }
    return output;
}





