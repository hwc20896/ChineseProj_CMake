#include "widget.h"
#include <QApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("data.db");
    if (database.open()) {
        Widget widget(database);
        widget.show();
        return app.exec();
    }
    return -1;
}