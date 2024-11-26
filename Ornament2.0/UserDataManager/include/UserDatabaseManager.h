#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QThread>
#include <QSqlError>
#include <QTimer>
#include <QRandomGenerator>
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
	void selectUserHeadData(const QString& userAccount);
	void closeDatabase();
	void VerifyUserAcocunt(const QString& userAccount, const  QString& userPassword);
	bool isExistTheSameUserAccount(const int account);
	void RegisterUserAccount(const QByteArray& imageBytes, const QString& userName, const QString& userPassword);
	void selectUserDataForSearch(const QString& userAccount);
	bool isExistTheSameUserApplication(const QString& receiver);
	void increaseUserApplicationTemp(const QString& receiver);
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
	void RegisterSucceedSignal(const int& account);
	void SearchFriendDataSignal(const SearchFriendData& data);
	void SendApplicationToServer(const QString& receiver);
	void SendApplicationSucceedSignal();
private:
	QPixmap pixmap_temp;
};
