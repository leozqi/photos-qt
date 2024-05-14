#include "datamodel.h"

DataModel::DataModel(QObject *parent)
    : QAbstractListModel{parent}
{

}


DataModel::~DataModel()
{

}


int DataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    // Return rows of photos in database
    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery q("SELECT COUNT(*) FROM photos", db);
    q.next();
    return q.value(0).toInt();
}


QVariant DataModel::data(const QModelIndex &index, int role) const
{
    // the index returns the requested row and column information.
    // we ignore the column and only use the row information
    int row = index.row()+1; // SQLite starts at 1

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q(db);
    q.prepare("SELECT id FROM photos WHERE id = (?)");
    q.addBindValue(row);
    if (!q.exec()) {
        qDebug() << "Failed to execute query: " << q.lastError().text() << " query was: " << q.executedQuery();
    }
    if (!q.next())
        return QVariant();

    switch (role) {
        case Qt::DisplayRole:
            return QVariant(q.value(0));
    }

    return QVariant();
}
