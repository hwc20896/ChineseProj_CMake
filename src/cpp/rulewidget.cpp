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

void RuleWidget::setQuantity(const int quantity, const int displayCount) const {
    ui->ruleInfo->setText(QString("題目庫共有%1條，每次開始遊戲系統會隨機抽取%2條顯示。").arg(QString::number(quantity), QString::number(displayCount)));
}