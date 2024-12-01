#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QThread>
#include <QPainter>
#include <QEvent>
#include <QPropertyAnimation>
#include "../../FramelessWindow/include/framelesswindow.h"
#include "../../global.h"
#include"../../Component/include/NormalTitleBar.h"
#include "../../UserDataManager/include/UserDatabaseManager.h"
#include "../Component/include/LineEditComponent.h"
#include "../../Component/include/RoundImage.h"
#include "../Component/include/ButtonComponent.h"
#include "../Register/include/RegisterUserAccount.h"
#include "../../Component/include/Notification.h"

class Login : public FramelessWindow
{
	Q_OBJECT

public:
	Login(QWidget* parent = Q_NULLPTR);
	~Login();
protected:
	void paintEvent(QPaintEvent*event)Q_DECL_OVERRIDE;
	void getUserHeadBytes(const QPixmap& userhead);
	QPixmap setPixmapDpi(const QSize& size, const QPixmap& pixmap);
	bool eventFilter(QObject* target, QEvent* event)Q_DECL_OVERRIDE;
	void deleteRegisterWindow();
	void closeEvent(QCloseEvent*event)Q_DECL_OVERRIDE;
	void deleteSqlThread();
	void VerifyFailed();

private:
	NormalTitleBar* login_title_Bar = Q_NULLPTR;
	QLabel* userHead = Q_NULLPTR;
	UserDatabaseManager* userDatabase = Q_NULLPTR;
	QThread* sql_thread = Q_NULLPTR;
	QPixmap userHeadPixmap;
	LineEditComponent* userAccountEdit = Q_NULLPTR;
	LineEditComponent* userPasswordEdit = Q_NULLPTR;
	ButtonComponent* loginButton = Q_NULLPTR;
	QLabel* registerButton = Q_NULLPTR;
	RegisterUserAccount* registerUserAccount = Q_NULLPTR;
	QPropertyAnimation* login_button_animation = Q_NULLPTR;
	bool isLogining = false;
	Notification* login_notification = Q_NULLPTR;
	QPropertyAnimation* login_notification_animation = Q_NULLPTR;

signals:
	void startloginAccountSignal(const QString& userAccount, const QString& password);
	//	void VerifySucceed(const QByteArray& imagebytes, const QString& userName, const  int& userAccount);
	void VerifySucceed(const QPixmap& userhead_pixmap, const QByteArray& imagebytes, const QString& userName, const  int& userAccount);
};
