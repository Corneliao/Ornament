#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPainter>
#include <QEvent>
#include <QFileDialog>
#include <QBuffer>
#include <QTimeLine>
#include "../../../Component/include/NormalTitleBar.h"
#include "../../../UserDataManager/include/UserDatabaseManager.h"
#include "../../Component/include/LineEditComponent.h"
#include "../../Component/include/ButtonComponent.h"
#include  "../../../FramelessWindow/include/framelesswindow.h"
#include "../../../Component/include/SystemNotification.h"
class RegisterUserAccount : public FramelessWindow
{
	Q_OBJECT

public:
	RegisterUserAccount(QWidget* parent);
	~RegisterUserAccount();
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
	bool eventFilter(QObject* target, QEvent* event) Q_DECL_OVERRIDE;
	void closeEvent(QCloseEvent*)Q_DECL_OVERRIDE;
private:
	NormalTitleBar* titleBar = Q_NULLPTR;
	QLabel* userHead = Q_NULLPTR;
	UserDatabaseManager* userDatabase = Q_NULLPTR;
	QThread* sql_thread = Q_NULLPTR;
	QPixmap userHeadPixmap;
	LineEditComponent* userName = Q_NULLPTR;
	LineEditComponent* userPassword = Q_NULLPTR;
	ButtonComponent* registerButton = Q_NULLPTR;
	QByteArray imagebytes;
	SystemNotification* notification = Q_NULLPTR;
	QTimeLine* animation = Q_NULLPTR;
signals:
	void closeWindowSignal();
	void registerUserAccount(const QByteArray& imagebytes, const QString& userName, const QString& userPassword);
};
