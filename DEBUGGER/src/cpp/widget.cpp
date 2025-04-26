#include "widget.h"
#include <QApplication>
#include "questioncontainer.h"
#include "viewer.h"

Widget::Widget(const QSqlDatabase& database, QWidget* parent) : QWidget(parent), ui(new Ui::mainForm) {
    ui->setupUi(this);
    connect(ui->exitButton, &QPushButton::clicked, this, &QApplication::quit);
    connect(ui->questionListDebugButton, &QPushButton::clicked, this, [this, database] {
        const auto container = new QuestionContainer(database);
        this->close();
        this->isMaximized()? container->showMaximized(): container->showNormal();
    });

    //  Styles
    ui->questionListDebugButton->setObjectName("navigator");
    ui->propertyDebugButton->setObjectName("navigator");
    ui->exitButton->setObjectName("navigator");
    this->setStyleSheet(Viewer::getStyle(":/styles/src/css/regular.css"));
}

Widget::~Widget() {
    delete ui;
}