#include "rulewidget.h"

RuleWidget::RuleWidget(QWidget* parent) : QWidget(parent), ui(new Ui::RuleForm){
    ui->setupUi(this);

    //  Connection
    connect(ui->returnButton, &QPushButton::clicked, this, &RuleWidget::toIntroPage);
}

RuleWidget::~RuleWidget(){delete ui;}