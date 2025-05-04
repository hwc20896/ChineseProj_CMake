#include "mainwidget.h"
#include <QApplication>
#include <QSqlDatabase>

int main(int argc, char** argv){
    QApplication app(argc, argv);
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLite");
    database.setDatabaseName("data.db");
    if (database.open()) {
        MainWidget window;
        window.resize(1000,700);
        window.show();
        return app.exec();
    }
    return 3;
}