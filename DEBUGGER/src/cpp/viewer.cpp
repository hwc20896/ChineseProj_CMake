#include "viewer.h"
#include <ranges>

Viewer::Viewer(const QString& target, const QSqlDatabase& database, QWidget* parent) : QWidget(parent), ui(new Ui::QuestionViewForm) {
    ui->setupUi(this);
    m_query = QSqlQuery(database);
    m_query.prepare("SELECT * FROM QuestionData WHERE QuestionTitle = ?");
    m_query.addBindValue(target);
    m_query.exec();
    m_query.next();

    //  Title
    ui->questionTitle->setText(target);

    //  Options

}

Viewer::~Viewer() {
    delete ui;
}