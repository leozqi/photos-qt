#include "photointerface.h"

PhotoInterface::PhotoInterface(QObject *parent)
    : QObject{parent}
{}


void PhotoInterface::openAlbum(const QString &path)
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open()) {
        qDebug() << "Could not open photo album";
    }
}

