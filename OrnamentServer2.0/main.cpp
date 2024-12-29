#include <QtCore/QCoreApplication>
#include "include/OrnamentServer.h"
#include <string.h>
#include <iostream>
class App : public  QThread {
public:
	App(QObject* parent = Q_NULLPTR);
protected:
	void run()Q_DECL_OVERRIDE;
};

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);
	App* thread = new App;
	QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater,Qt::DirectConnection);
	QObject::connect(thread, &QThread::finished, &a, &QCoreApplication::quit,Qt::QueuedConnection);
	thread->start();
	OrnamentServer server;
	return a.exec();
}

App::App(QObject* parent)
{
}

void App::run()
{
	char ch;
	while (1) {
		ch = getchar();
		if (ch == 'q') {
			break;
		}
	}
}
