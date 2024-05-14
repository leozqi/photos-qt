#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuick>

#include "photointerface.h"
#include "datamodel.h"
#include "imageprovider.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/photos/Main.qml"));

    qmlRegisterType<DataModel>("com.leozqi.photos", 1, 0, "DataModel");
    qmlRegisterSingletonType<PhotoInterface>("com.leozqi.photos", 1, 0, "PhotoInterface", &PhotoInterface::qmlInstance);

    engine.addImageProvider(QLatin1String("imageprovider"), new ImageProvider);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

