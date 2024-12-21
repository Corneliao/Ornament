//#include "QuickFramelessWindow/QuickFramelessWindow.h"
#include "QuickFramelessWindow/QuickFramelessWindow.h"
#include "include/ImageLoader.h"
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include "include/global.h"
#include "include/Login.h"
#include <QScopedPointer>
int main(int argc, char *argv[]) {
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGLRhi);
    QGuiApplication app(argc, argv);

    qmlRegisterType<QuickFramelessWindow>("Qt.FramelessWindow", 1, 0, "FramelessWindow");
    qmlRegisterType<ImageLoader>("Qt.ImageLoader", 1, 0, "ImageLoader");

    int font_id = QFontDatabase::addApplicationFont(":/resource/font/flache.ttf");
    if (font_id != -1) {
        QString family = QFontDatabase::applicationFontFamilies(font_id).at(0);
        QFont font;
        font.setHintingPreference(QFont::PreferNoHinting);
        font.setFamily(family);
        app.setFont(font);
    }


    Login  * login = new Login;
    QObject::connect(login,&Login::succeed,&app,[&app, &login](){
            login->destoryQuickView();
            delete login;
            Sleep(700);
            global *  glo = new global;
            glo->setWindowDpi(app.devicePixelRatio());
            QQmlApplicationEngine   *engine = new  QQmlApplicationEngine(Q_NULLPTR);
            QObject::connect(
                    engine,
                    &QQmlApplicationEngine::objectCreationFailed,
                    &app,
                    []() { QCoreApplication::exit(-1); },
                    Qt::QueuedConnection);

             engine->rootContext()->setContextProperty("global",glo);
             engine->load("qrc:/resource/qml/Main.qml");
             QObject * object = engine->rootObjects().at(0);

             QObject::connect(object,&QObject::objectNameChanged,[=](const QString & name){
                 object->deleteLater();
                 engine->deleteLater();
             });
    });
//    global glo;
//    glo.setWindowDpi(app.devicePixelRatio());
//
//    QQmlApplicationEngine engine;
//    engine.rootContext()->setContextProperty("global",&glo);
//    QObject::connect(
//            &engine,
//            &QQmlApplicationEngine::objectCreationFailed,
//            &app,
//            []() { QCoreApplication::exit(-1); },
//            Qt::QueuedConnection);
//    engine.load("qrc:/resource/qml/LoginContainer.qml");

    return QGuiApplication::exec();
}
