#ifndef PHOTOINTERFACE_H
#define PHOTOINTERFACE_H

#include <QObject>
#include <QQuickItem>
#include <QtSql/QSqlDatabase>

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

    void openAlbum(const QString &path);

signals:

private:
    QSqlDatabase db;
};

#endif // PHOTOINTERFACE_H
