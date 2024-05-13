#ifndef PHOTOINTERFACE_H
#define PHOTOINTERFACE_H

#include <QObject>
#include <QQuickItem>
#include <QtSql/QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QFile>
#include <QImage>
#include <QBuffer>

#include <exiv2/exiv2.hpp>

class PhotoInterface : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit PhotoInterface(QObject *parent = nullptr);

    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine);
        Q_UNUSED(scriptEngine);

        return new PhotoInterface;
    }

public slots:
    void openAlbum(const QString &path);
    void addPhotos(const QList<QString> &paths);

private:
    QString currentAlbum;
};

#endif // PHOTOINTERFACE_H
