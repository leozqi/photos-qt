#include "photointerface.h"

PhotoInterface::PhotoInterface(QObject *parent)
    : QObject{parent}
{}


void PhotoInterface::openAlbum(const QString &path)
{
    // The file path is both the connection name and the "database name"
    this->db = QSqlDatabase::addDatabase("QSQLITE", path);
    db.setDatabaseName(path);

    if (!db.open()) {
        qDebug() << "Could not open photo album";
    }
}

