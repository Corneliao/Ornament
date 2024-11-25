#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QThread>
#include <QSqlError>
#include <QTimer>
#include "../../global.h"
#include "../../Component/include/RoundImage.h"
class UserDatabaseManager : public QObject
{
	Q_OBJECT

public:
	UserDatabaseManager(const QString& connect_name);
	~UserDatabaseManager();
public:
	void iniSql();
	void selectUserData(const QString& userAccount);
	void closeDatabase();
	void VerifyUserAcocunt(const QString& userAccount, const  QString& userPassword);
protected:
private:
	QString connectName = "connected_forlogin";
signals:
	void closedDatabaseSignal();
	void userHeadByteArray(const QPixmap& userhead);
	//void VerifySucceed(const QByteArray& imagebytes, const QString& userName, const  int& userAccount);
	void VerifySucceed(const QPixmap& userhead_pixmap, const QByteArray& imagebytes, const QString& userName, const  int& userAccount);
	void VerifyFailed(bool flag);
	void UnValidUserAccount(bool enable);
private:
	QPixmap pixmap_temp;
};
