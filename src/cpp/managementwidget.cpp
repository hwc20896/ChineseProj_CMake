#include "managementwidget.h"
#include <QFile>
#include <random>
#include <QJsonDocument>
#include <QJsonArray>
#include <algorithm>

ManagementWidget::ManagementWidget(const bool currentMuted, QWidget* parent) : QWidget(parent), m_ui(new Ui::ManagementWidget) {
    data = deserializeJson();
    assert(data.size() > 0);
    isMuted = currentMuted;

    //  BGM
    output = new QAudioOutput();
    output->setVolume(.15);
    player = new QMediaPlayer();
    player->setAudioOutput(output);
    player->setSource({"qrc:/BGM/medias/OMFG_Pizza.mp3"});
    connect(player, &QMediaPlayer::mediaStatus, this, [this](const QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::LoadedMedia) player->play();
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
            result.push_back(QuestionData(value.toObject()));
        });
    }
    return result;
}


