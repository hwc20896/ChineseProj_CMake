#include "questioncontainer.h"

QuestionContainer::QuestionContainer(const QSqlDatabase& database, QWidget* parent) : QWidget(parent), ui(new Ui::QuestionListForm), m_database(database) {
    ui->setupUi(this);
    m_query = QSqlQuery(database);

    ui->questionContainer->addItems(getQuestionList());
}

QuestionContainer::~QuestionContainer() {
    delete ui;
    m_database.close();
}

QStringList QuestionContainer::getQuestionList(){
    QStringList output;
    m_query.exec("SELECT QuestionTitle FROM QuestionData");
    while (m_query.next())
        output.push_back(m_query.value(0).toString());
    return output;
}
