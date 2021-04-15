#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "analysis.h"
#include "TableModel.h"
#include "AHPModel.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    qmlRegisterType<TableModel>("TableModel", 0, 1, "TableModel");
    qmlRegisterType<AHPModel>("AHPModel", 0, 1, "AHPModel");

    Analysis analysis;

    QQmlApplicationEngine engine;
    QQmlContext* context=engine.rootContext();
    context->setContextProperty("CSI_Analysis" , &analysis);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
