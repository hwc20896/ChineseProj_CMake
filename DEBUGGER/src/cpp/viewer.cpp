#include "viewer.h"
#include <ranges>
#include <map>
#include <array>
#include <QPushButton>

Viewer::Viewer(const QString& target, const QSqlDatabase& database, QWidget* parent) : QWidget(parent), ui(new Ui::QuestionViewForm) {
    difficulty_to_color = {
        {0, "[<font color=#00dd00>簡單</font>] %1. %2"},
        {1, "[<font color=#ff0000>普通</font>] %1. %2"},
        {2, "[<font color=#ff00ff>困難</font>] %1. %2"}
    };

    ui->setupUi(this);
    m_query = QSqlQuery(database);
    m_query.prepare("SELECT * FROM QuestionData WHERE QuestionTitle = ?");
    m_query.addBindValue(target);
    m_query.exec();
    m_query.next();

    //  Title
    const auto difficulty = m_query.value(8).toInt();
    ui->questionTitle->setText(QString(difficulty_to_color[difficulty]).arg(
        m_query.value(0).toString(), m_query.value(1).toString())
    );


    //  Options
    constexpr auto indexes = std::views::iota(2,6);
    for (const auto bind_buttons = {ui->optionA, ui->optionB, ui->optionC, ui->optionD};
        auto [button, index] : std::views::zip(bind_buttons, indexes)
    ) {
        index_to_button.emplace(index, button);
        const auto temp = m_query.value(index);
        temp.isNull()? button->hide() : button->setText(temp.toString());
    }
}

Viewer::~Viewer() {
    delete ui;
}