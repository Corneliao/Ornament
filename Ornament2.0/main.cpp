#include "include/Ornament.h"
#include <QtWidgets/QApplication>
#include <QScreen>
#include <QFontDatabase>
#include "Login/include/Login.h"
int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	int font_id = QFontDatabase::addApplicationFont(":/Resource/font/PICOSC Harmony Manrope-VF.ttf");
	QString family = QFontDatabase::applicationFontFamilies(font_id).at(0);
	QFont font;
	font.setHintingPreference(QFont::PreferNoHinting);
	font.setFamily(family);
	a.setFont(font);
	QScreen* primaryScreen = a.primaryScreen();
	screenSize = primaryScreen->size();
	qDebug() << primaryScreen->size();
	Login login;
	QPoint center = QPoint((primaryScreen->size().width() - login.width()) / 2, (primaryScreen->size().height() - login.height()) / 2);
	login.move(center);
	login.show();

	QObject::connect(&login, &Login::VerifySucceed, [&](const QPixmap& userhead_pixmap, const QByteArray& imagebytes, const QString& userName, const  int& userAccount) {
		login.close();
		Ornament* orname = new Ornament(userhead_pixmap, imagebytes, userName, userAccount);
		QPoint p = QPoint((primaryScreen->size().width() - orname->width()) / 2, (primaryScreen->size().height() - orname->height()) / 2);
		orname->move(p);
		Sleep(700);
		orname->show();
		});
	return a.exec();
}