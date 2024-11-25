#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPainter>
#include "../../include/LoginTitleBar.h"
#include "../../../UserDataManager/include/UserDatabaseManager.h"
#include "../../Component/include/LineEditComponent.h"
#include "../../Component/include/ButtonComponent.h"
#include  "../../../FramelessWindow/include/framelesswindow.h"
class RegisterUserAccount : public FramelessWindow
{
	Q_OBJECT

public:
	RegisterUserAccount(QWidget* parent);
	~RegisterUserAccount();
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	LoginTitleBar* login_title_Bar = Q_NULLPTR;
	QLabel* userHead = Q_NULLPTR;
	UserDatabaseManager* userDatabase = Q_NULLPTR;
	QThread* sql_thread = Q_NULLPTR;
	QPixmap userHeadPixmap;
	LineEditComponent* userAccountEdit = Q_NULLPTR;
	LineEditComponent* userPasswordEdit = Q_NULLPTR;
	ButtonComponent* loginButton = Q_NULLPTR;
signals:
	void closeWindowSignal();
};
