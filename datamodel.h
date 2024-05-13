#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QtCore>
#include <QtGui>

class DataModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DataModel(QObject *parent = nullptr);
    ~DataModel();

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

signals:
};

#endif // DATAMODEL_H
