#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QQuickItem>
#include <QtCore>
#include <QtGui>
#include <QtSql/QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <exiv2/exiv2.hpp>

class DataModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit DataModel(QObject *parent = nullptr);
    ~DataModel();

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

public slots:
    void openAlbum(const QString &path);
    void addPhotos(const QList<QString> &paths);
};

#endif // DATAMODEL_H
