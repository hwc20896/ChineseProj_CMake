#include "mainwidget.h"

MainWidget::MainWidget(QWidget* parent) : QStackedWidget(parent), intro(new IntroWidget), rule(new RuleWidget){
    this->addWidget(intro);
    this->addWidget(rule);
    this->setCurrentWidget(intro);

    //  Connections
    connect(intro, &IntroWidget::toRulePage, this, [this]{this->setCurrentWidget(rule);});
    connect(rule, &RuleWidget::toIntroPage, this, [this]{this->setCurrentWidget(intro);});
}

MainWidget::~MainWidget(){
    delete intro;
    delete rule;
}
