#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "QuickFramelessWindow.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    qmlRegisterType<QuickFramelessWindow>("Qt.QuickFramelessWindow", 1, 0, "FramelessWindow");
    QQmlApplicationEngine engine;
    QObject::connect(
            &engine,
            &QQmlApplicationEngine::objectCreationFailed,
            &app,
            []() { QCoreApplication::exit(-1); },
            Qt::QueuedConnection);
    engine.load(QUrl("qrc:/Main.qml"));

    return QGuiApplication::exec();
}
