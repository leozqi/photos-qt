#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QtSql/QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider()
        : QQuickImageProvider(QQuickImageProvider::Pixmap)
    {
    }

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override
    {
        int width = 200;
        int height = 150;

        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery q(db);
        q.prepare("SELECT thumbnail FROM photos WHERE id = (?)");
        q.addBindValue(id.toInt());
        if (!q.exec()) {
            qDebug() << "Failed to execute query: " << q.lastError().text() << " query was: " << q.executedQuery();
        }
        q.next();

        QImage img = QImage::fromData(q.value(0).toByteArray());
        return img;
    }
};

#endif // IMAGEPROVIDER_H
