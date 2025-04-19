#include "widget.h"
#include <QApplication>

Widget::Widget(const QSqlDatabase& database, QWidget* parent) : QWidget(parent), m_database(database), ui(new Ui::mainForm) {
    m_query = QSqlQuery(m_database);

    ui->setupUi(this);
    connect(ui->exitButton, &QPushButton::clicked, this, &QApplication::quit);
}

Widget::~Widget() {
    m_database.close();
    delete ui;
}

