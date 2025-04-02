#include "introwidget.h"

IntroWidget::IntroWidget(QWidget* parent) : QWidget(parent), ui(new Ui::IntroForm){
    ui->setupUi(this);

    //  Connections
    connect(ui->rule, &QPushButton::clicked, this, &IntroWidget::toRulePage);
    connect(ui->startGame, &QPushButton::clicked, this, &IntroWidget::startGame);
}

IntroWidget::~IntroWidget(){delete ui;}