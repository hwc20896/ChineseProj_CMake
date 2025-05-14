#include "property.h"

Property::Property(const QSqlDatabase& database, QWidget* parent) : QWidget(parent), ui(new Ui::Property) {
    ui->setupUi(this);
    m_query = QSqlQuery(database);
}

Property::~Property() {
    delete ui;
}
