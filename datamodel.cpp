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


void DataModel::openAlbum(const QString &path)
{
    // The file path is both the connection name and the "database name"
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open()) {
        qDebug() << "Could not open photo album";
        return;
    }

    QSqlQuery q(db);
    q.prepare("CREATE TABLE IF NOT EXISTS photos ("
              "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
              "path TEXT NOT NULL, "
              "image BLOB NOT NULL, "
              "thumbnail BLOB NOT NULL);"
              );
    q.exec();

    q.prepare("CREATE TABLE IF NOT EXISTS metadata ("
              "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
              "photo_id INTEGER NOT NULL, "
              "key TEXT NOT NULL, "
              "value TEXT NOT NULL, "
              "CONSTRAINT fk_photos FOREIGN KEY (photo_id) REFERENCES photos(id) ON DELETE CASCADE);"
              );
    if (!q.exec()) {
        qDebug() << "Failed to create metadata table";
    }
}


static const Exiv2::TagInfo* findTag(const Exiv2::TagInfo* pList, uint16_t tag) {
    while (pList->tag_ != 0xffff && pList->tag_ != tag)
        pList++;
    return pList->tag_ != 0xffff ? pList : nullptr;
}


void DataModel::addPhotos(const QList<QString> &paths)
{
    QSqlDatabase db = QSqlDatabase::database();
    QByteArray imgData;
    QBuffer imgBuffer(&imgData);
    imgBuffer.open(QIODevice::WriteOnly);
    QByteArray thumbData;
    QBuffer thumbBuffer(&thumbData);
    thumbBuffer.open(QIODevice::WriteOnly);

    QSqlQuery q(db);

    int beforeSize = rowCount(QModelIndex());

    emit beginInsertRows(QModelIndex(), beforeSize, beforeSize+static_cast<int>(paths.count()));
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
        QImage scaled = img.scaled(200, 150, Qt::KeepAspectRatio, Qt::FastTransformation);

        img.save(&imgBuffer, "PNG");
        scaled.save(&thumbBuffer, "PNG");

        q.prepare("INSERT INTO photos (path, image, thumbnail) VALUES (?, ?, ?)");
        q.addBindValue(paths.at(i));
        q.addBindValue(imgData);
        q.addBindValue(thumbData);
        if (!q.exec()) {
            qDebug() << "Failed to execute query: " << q.lastError().text() << " query was: " << q.executedQuery();
        }
        QVariant lastInserted = q.lastInsertId();

        imgBuffer.buffer().clear();
        imgBuffer.seek(0);
        thumbBuffer.buffer().clear();
        thumbBuffer.seek(0);

        // Load EXIF data
        auto image = Exiv2::ImageFactory::open(localPath.toStdString());
        image->readMetadata();

        Exiv2::ExifData& exifData = image->exifData();
        if (exifData.empty()) {
            qDebug() << "No exifData found";
        } else {
            for (auto i = exifData.begin(); i != exifData.end(); ++i) {
                const Exiv2::TagInfo* tagInfo = findTag(Exiv2::ExifTags::tagList(i->groupName()), i->tag());
                if (!tagInfo)
                    continue;

                q.prepare("INSERT INTO metadata (photo_id, key, value) VALUES (?, ?, ?)");
                q.addBindValue(lastInserted);
                q.addBindValue(QString::fromStdString(i->key()));
                q.addBindValue(QString::fromStdString(i->toString()));
                if (!q.exec()) {
                    qDebug() << "Failed to execute query: " << q.lastError().text() << " query was: " << q.executedQuery();
                }
            }
        }
    }

    emit endInsertRows();
}
