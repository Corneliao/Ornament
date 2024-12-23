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
#include "include/Client.h"
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

    //系统窗口缩放比例
    Login login(app.devicePixelRatio());

    QObject::connect(&login,&Login::UserLogined,&app,[&app,&login](const QString & userName,const QString  &account,const QString & imageData){
            login.destoryQuickView();
            Sleep(700);
            auto *  glo = new global;
            glo->setUserHead(imageData);
            glo->setUserName(userName);
            glo->setUserAccount(account);
            glo->setWindowDpi(app.devicePixelRatio());
           // glo->addFrindInfo(imageData,account);

            auto * client = new Client;
            auto   *engine = new  QQmlApplicationEngine(Q_NULLPTR);
            QObject::connect(
                    engine,
                    &QQmlApplicationEngine::objectCreationFailed,
                    &app,
                    [=]() {QCoreApplication::exit(-1);},
                    Qt::QueuedConnection);

             engine->rootContext()->setContextProperty("global",glo);
             engine->rootContext()->setContextProperty("kk",client);
             engine->load("qrc:/resource/qml/Main.qml");
             QObject * object = engine->rootObjects().at(0);

             QObject::connect(object,&QObject::objectNameChanged,[=](const QString & name){
                 Q_UNUSED(name);
                 object->deleteLater();
                 engine->deleteLater();
                 glo->deleteLater();
                 client->deleteLater();
             });
    });

    return QGuiApplication::exec();
}
