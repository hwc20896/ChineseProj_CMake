#include "widget.h"

Widget::Widget(const QSqlDatabase& database, QWidget* parent) : QWidget(parent), m_database(database) {
    m_query = QSqlQuery(m_database);

    //   Game DATA
    m_query.exec("CREATE TABLE IF NOT EXISTS QuestionData ("
                 "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "QuestionTitle TEXT NOT NULL, "
                 "Option1 TEXT NOT NULL, "
                 "Option2 TEXT NOT NULL, "
                 "Option3 TEXT, "
                 "Option4 TEXT, "
                 "CorrectOption INTEGER NOT NULL, "
                 "DIFFICULTY INTEGER NOT NULL, "
                 "Description TEXT, "
                 "Hint TEXT) ");

    //   Game CONFIG
    m_query.exec("CREATE TABLE IF NOT EXISTS AppConfig ("
                 "AppTitle TEXT NOT NULL, "
                 "GameTitle TEXT NOT NULL, "
                 "DefaultBGMMute BOOLEAN NOT NULL, "
                 "DefaultEffectMute BOOLEAN NOT NULL, "
                 "ToggleHardmodeEnabled BOOLEAN NOT NULL, "
                 "DisplayQuantity INTEGER NOT NULL, "
                 "HardmodeCountdown INTEGER NOT NULL) ");


}

Widget::~Widget() {
    m_database.close();
}

