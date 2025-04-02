#include "mainwidget.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

MainWidget::MainWidget(QWidget* parent) : QStackedWidget(parent), intro(new IntroWidget), rule(new RuleWidget){
    this->addWidget(intro);
    this->addWidget(rule);
    this->setCurrentWidget(intro);

    //  Connections
    connect(intro, &IntroWidget::toRulePage, this, [this]{this->setCurrentWidget(rule);});
    connect(rule, &RuleWidget::toIntroPage, this, [this]{this->setCurrentWidget(intro);});
    connect(intro, &IntroWidget::startGame, this, &MainWidget::startGame);

    //  Json parsing
    if (QFile propertyFile("properties.json"); propertyFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        auto doc = QJsonDocument::fromJson(propertyFile.readAll());
        propertyFile.close();
        assert(doc.isObject());
        auto object = doc.object();

        //  Applying json configs
        this->appTitle  = object["app_title"].toString("某游戲");
        this->gameTitle = object["game_title"].toString("游戲標題");
        this->isMuted   = object["default_muted"].toBool(true);
        
        this->setWindowTitle(this->appTitle);
    }
}

MainWidget::~MainWidget(){
    delete intro;
    delete rule;
}

void MainWidget::startGame(){
    
}