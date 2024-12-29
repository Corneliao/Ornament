#include "../include/UserDatabaseManager.h"

UserDatabaseManager::UserDatabaseManager(QObject* parent)
	: QObject(parent)
{
}

UserDatabaseManager::~UserDatabaseManager()
{
	this->updateStat();
	this->closeDatabase();
	qDebug() << __FUNCTION__ << __TIME__ << "数据库线程被销毁：" << QThread::currentThreadId();
}

void UserDatabaseManager::initialSql()
{
	this->connect_name = QString("sql_") + QString::number((quintptr)QThread::currentThreadId());
	qDebug() << __FUNCTION__ << __TIME__ << "数据库子线程" << QThread::currentThreadId();
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", this->connect_name);
	db.setUserName("root");
	db.setPassword("y..750242");
	db.setPort(3306);
	db.setDatabaseName("user_1");
	db.setHostName("localhost");
	if (db.open()) {
		qInfo() << __FUNCTION__ << __TIME__ << "打开数据库成功";
	}

	else {
		qDebug() << db.lastError();
		return;
	}
}

void UserDatabaseManager::closeDatabase() const
{
	qDebug() << __FUNCTION__ << this->connect_name << "关闭数据库";
	{
		QSqlDatabase db = QSqlDatabase::database(this->connect_name);
		db.close();
	}
	QSqlDatabase::removeDatabase(this->connect_name);
}

void UserDatabaseManager::updateClientStatus(const QString& userAccount, const QString& type)
{
	this->m_account = userAccount;

	QSqlDatabase db = QSqlDatabase::database(this->connect_name);
	QSqlQuery query(db);
	if (query.exec("UPDATE usersinfo SET onlineStatus = " + type + " WHERE userAccount = " + this->m_account)) {
		qDebug() << "更新状态成功";
	}
	else {
		qDebug() << query.lastError();
	}
}

void UserDatabaseManager::updateStat()
{
	QSqlDatabase db = QSqlDatabase::database(this->connect_name);
	QSqlQuery query(db);
	if (query.exec("UPDATE usersinfo SET onlineStatus = 0 WHERE userAccount = " + this->m_account)) {
		qDebug() << "更新状态成功";
	}
	else {
		qDebug() << query.lastError();
	}
}

void UserDatabaseManager::isExistTheUser(const QString& userAccount)
{
	if (this->m_account == userAccount)
		return;
	QSqlDatabase db = QSqlDatabase::database(this->connect_name);
	QSqlQuery query(db);
	query.prepare("SELECT friendAccount FROM userfriends WHERE userAccount = " + this->m_account);
	if (query.exec()) {
		while (query.next()) {
			QString account = query.value(0).toString();
			if (account == userAccount) {
				emit this->isEixstTheUserSignal(userAccount, true);
			}
		}
	}
	else {
		qDebug() << query.lastError();
	}
	emit this->isEixstTheUserSignal(userAccount, false);
}

bool UserDatabaseManager::isExistTheUserForDatabase(const QString& userAccount)
{
	if (this->m_account == userAccount)
		return true;
	QSqlDatabase db = QSqlDatabase::database(this->connect_name);
	QSqlQuery query(db);
	query.prepare("SELECT friendAccount FROM userfriends WHERE userAccount = " + this->m_account);
	if (query.exec()) {
		while (query.next()) {
			QString account = query.value(0).toString();
			if (account == userAccount) {
				return true;
			}
		}
	}
	else {
		qDebug() << query.lastError();
	}
	return false;
}

void UserDatabaseManager::addNewUser(const QString& sender, const QString& receiver)
{
	QSqlDatabase db = QSqlDatabase::database(this->connect_name);
	QSqlQuery query(db);

	//查询申请列表
	query.prepare("SELECT sender,receiver FROM friendapplication WHERE sender = " + sender + " AND receiver = " + receiver);
	if (query.exec())
	{
		int sender_account;
		int receiver_account;
		while (query.next()) {
			sender_account = query.value(0).toInt();
			receiver_account = query.value(1).toInt();
		}
		//添加至好友列表
		query.prepare("INSERT INTO userfriends (userAccount,friendAccount) VALUES (:user,:crony)");
		query.bindValue(":user", sender.toInt());
		query.bindValue(":crony", receiver.toInt());
		bool ret = query.exec();
		if (!ret) {
			qDebug() << query.lastError();
		}

		query.prepare("INSERT INTO userfriends (userAccount,friendAccount) VALUES (:user,:crony)");
		query.bindValue(":user", receiver.toInt());
		query.bindValue(":crony", sender.toInt());

		if (query.exec() && ret) {
			qDebug() << "添加好友成功";
			emit this->addNewUserSucceed(sender);
		}

		else
			qDebug() << query.lastError();

		//删除申请记录
		query.prepare("DELETE FROM friendapplication WHERE sender = " + sender + " AND receiver = " + receiver);
		if (!query.exec())
			qDebug() << query.lastError();
		qDebug() << __TIME__ << "删除记录";
	}
	else {
		qDebug() << query.lastError();
	}
}

void UserDatabaseManager::isMyFirend(const QString& senderUserAccount, const QString& reiceiverUserAccount, const QString& message)
{
	//QSqlDatabase db = QSqlDatabase::database(this->connect_name);
	//QSqlQuery query(db);
	//query.prepare("SELECT cronyAccount FROM UserCrony WHERE userAccount = " + this->m_account);
	//if (query.exec()) {
	//	while (query.next()) {
	//		QString account = query.value(0).toString();
	//		if (account == userAccount) {
	//			emit this->isEixstTheUserSignal(userAccount, true);
	//		}
	//	}
	//}
	//else {
	//	qDebug() << query.lastError();
	//}
	//emit this->isEixstTheUserSignal(userAccount, false);
}

void UserDatabaseManager::FindFriendForDisconnected(const QString& userAccount)
{
	QSqlDatabase db = QSqlDatabase::database(this->connect_name);
	QSqlQuery query(db);
	query.prepare("SELECT friendAccount FROM userfriends WHERE userAccount = " + this->m_account);
	if (query.exec()) {
		while (query.next()) {
			QString account = query.value(0).toString();
			if (account == userAccount) {
				emit this->FindFriendForDisconnectedSignal(userAccount, true);
			}
		}
	}
	else {
		qDebug() << query.lastError();
		emit this->FindFriendForDisconnectedSignal(userAccount, false);
	}
}