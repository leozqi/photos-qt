#include "photointerface.h"

PhotoInterface::PhotoInterface(QObject *parent)
    : QObject{parent}
{}


void PhotoInterface::openAlbum(const QString &path)
{
    // The file path is both the connection name and the "database name"
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", path);
    db.setDatabaseName(path);

    if (!db.open()) {
        qDebug() << "Could not open photo album";
        return;
    }

    currentAlbum = path;

    QSqlQuery q(db);
    q.prepare("CREATE TABLE IF NOT EXISTS photos ("
        "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
        "path TEXT NOT NULL, "
        "image BLOB NOT NULL, "
        "thumbnail BLOB NOT NULL);"
    );
    q.exec();
}


void PhotoInterface::addPhotos(const QList<QString> &paths)
{
    QSqlDatabase db = QSqlDatabase::database(currentAlbum);
    QBuffer imgBuffer;
    imgBuffer.open(QIODevice::WriteOnly);
    QBuffer thumbBuffer;
    thumbBuffer.open(QIODevice::WriteOnly);

    for (qsizetype i = 0; i < paths.size(); ++i) {
        QString localPath = QUrl(paths.at(i)).toLocalFile();

        // Guesses image format from header
        // Supported are:
        // BMP	image/bmp	Windows Bitmap
        // GIF	image/gif	Graphic Interchange Format (optional)
        // JPG	image/jpeg	Joint Photographic Experts Group
        // PNG	image/png	Portable Network Graphics
        // PBM	image/x-portable-bitmap	Portable Bitmap
        // PGM	image/x-portable-graymap	Portable Graymap
        // PPM	image/x-portable-pixmap	Portable Pixmap
        // XBM	image/x-xbitmap	X11 Bitmap
        // XPM	image/x-xpixmap	X11 Pixmap
        // SVG	image/svg+xml	Scalable Vector Graphics
        //
        // See: https://doc.qt.io/qt-6/qimagereader.html#supportedImageFormats
        QImage img(localPath);
        QImage scaled = img.scaled(200, 150, Qt::IgnoreAspectRatio, Qt::FastTransformation);

        img.save(&imgBuffer);
        scaled.save(&thumbBuffer);

        QSqlQuery q(db);
        q.prepare("INSERT INTO photos (path, image, thumbnail) VALUES (?, ?, ?)");
        q.addBindValue(paths.at(i));
        q.addBindValue(imgBuffer.buffer());
        q.addBindValue(thumbBuffer.buffer());

        if (!q.exec()) {
            qDebug() << "Failed to execute query: " << q.lastError().text() << " query was: " << q.executedQuery();
        }

        imgBuffer.buffer().clear();
        imgBuffer.seek(0);
        thumbBuffer.buffer().clear();
        thumbBuffer.seek(0);
    }
}
