#include "rulewidget.h"
#include "template.h"

RuleWidget::RuleWidget(QWidget* parent) : QWidget(parent), ui(new Ui::RuleForm){
    ui->setupUi(this);

    //  Connection
    connect(ui->returnButton, &QPushButton::clicked, this, &RuleWidget::toIntroPage);

    //  Styles
    ui->returnButton->setObjectName("navigator");
    this->setStyleSheet(QuestionWidget::getStyleFromURI(":/Stylesheets/src/css/regular.css"));
}

RuleWidget::~RuleWidget(){delete ui;}