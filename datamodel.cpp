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
    int row = index.row();

    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery q("SELECT COUNT(*) FROM photos", db);
    q.next();
    return q.value(0).toInt();


    // boundary check for the row
    //if(row < 0 || row >= m_data.count()) {
    //    return QVariant();
    //}

    // A model can return data for different roles.
    // The default role is the display role.
    // it can be accesses in QML with "model.display"
    switch(role) {
    case Qt::DisplayRole:
        // Return the color name for the particular row
        // Qt automatically converts it to the QVariant type
        //return m_data.value(row);
    }

    // The view asked for other data, just return an empty QVariant
    return QVariant();
}

