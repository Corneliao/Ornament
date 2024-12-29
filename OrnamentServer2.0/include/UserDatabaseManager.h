#pragma once

#include <QObject>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
class UserDatabaseManager : public QObject
{
	Q_OBJECT

public:
	UserDatabaseManager(QObject* parent = nullptr);
	~UserDatabaseManager();
public:
	void initialSql();
	void closeDatabase() const;
	void updateClientStatus(const QString& userAccount, const QString& type);
	void updateStat();
	void isExistTheUser(const QString& userAccount);
	bool isExistTheUserForDatabase(const QString& userAccount);
	void addNewUser(const QString& sender, const QString& receiver);
	void isMyFirend(const QString& senderUserAccount, const QString& reiceiverUserAccount, const QString& message);
	void FindFriendForDisconnected(const QString& userAccount);
private:
	QString connect_name;
	QString m_account;
signals:
	void isEixstTheUserSignal(const QString& userAccount, bool isExist);
	void addNewUserSucceed(const QString& senderAccount);
	void FindFriendForDisconnectedSignal(const QString& userAccount, bool isExist);
};
