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
	bool isExistTheSameUserApplication(const QString& receiver) const;
	void increaseUserApplicationTemp(const QString& receiver);
	bool isExistTheUser(const QString& userAccount);
	UserData GetUserFriendData(const QString& cronyAccount); //
	void selectCurrentUserFriends();
	void selectUserData(const QString& userAccount);

signals:
	void closedDatabaseSignal();
	void userHeadByteArray(const QPixmap& userhead);
	//void VerifySucceed(const QByteArray& imagebytes, const QString& userName, const  int& userAccount);
	void VerifySucceed(const QPixmap& userhead_pixmap, const QByteArray& imagebytes, const QString& userName, const  int& userAccount);
	void VerifyFailed();
	void UnValidUserAccount(bool enable);
	void RegisterSucceedSignal(const int& account);
	void SearchFriendDataSignal(const SearchFriendData& data);
	void SendApplicationToServer(const QString& receiver);
	void isSendApplication(bool isSucceed);
	void existTheUserSignal();
	void userFriends(QList<UserData> datas);
	void userDataSignal(const UserData& user_data);
	//void openDatabaseFailedSignal(const QString& error);
private:
	QPixmap pixmap_temp;
	QString connectName = "connected_forlogin";
};
