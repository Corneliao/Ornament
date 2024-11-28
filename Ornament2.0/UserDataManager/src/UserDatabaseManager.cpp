#include "../include/UserDatabaseManager.h"

UserDatabaseManager::UserDatabaseManager(const QString& connect_name)
{
	this->connectName = connect_name;
	qRegisterMetaType<QList<FriendListData>>("QList<FriendListData>");
}

UserDatabaseManager::~UserDatabaseManager()
{
	qDebug() << __FUNCTION__ << __TIME__ << "do work deleter";
	this->closeDatabase();
}

void UserDatabaseManager::closeDatabase()
{
	qDebug() << __FUNCTION__ << this->connectName << "关闭数据库";
	{
		QSqlDatabase db = QSqlDatabase::database(this->connectName);
		db.close();
	}
	QSqlDatabase::removeDatabase(this->connectName);
}

void UserDatabaseManager::VerifyUserAcocunt(const QString& userAccount, const QString& userPassword)
{
	{
		QSqlDatabase db = QSqlDatabase::database(this->connectName);
		QSqlQuery query(db);
		query.prepare("SELECT userAccount FROM UserInfo");
		if (query.exec()) {
			while (query.next()) {
				QString account = query.value(0).toString();
				if (account == userAccount) {
					query.prepare("SELECT userName,userHeadByte,userPassword FROM UserInfo WHERE userAccount = " + userAccount);
					if (query.exec()) {
						while (query.next()) {
							QString userName = query.value(0).toString();
							QByteArray imagebytes = query.value(1).toByteArray();
							QString password = query.value(2).toString();
							if (password == userPassword)
								emit this->VerifySucceed(this->pixmap_temp, imagebytes, userName, userAccount.toInt());
							else
								emit this->VerifyFailed();
							return;
						}
					}
				}
			}
			emit this->VerifyFailed();

		}
		else {
			emit this->VerifyFailed();
		}
	}
}

bool UserDatabaseManager::isExistTheSameUserAccount(const int account)
{
	{
		QSqlDatabase db = QSqlDatabase::database(this->connectName);
		QSqlQuery query(db);
		query.prepare("SELECT UserInfo.userAccount FROM UserInfo;");
		if (query.exec()) {
			while (query.next()) {
				int _account = query.value(0).toInt();
				if (_account == account)
					return true;
			}
			return false;
		}
		else {
			qDebug() << __FUNCTION__ << __TIME__ << query.lastError();
			return false;
		}
	}
}

void UserDatabaseManager::RegisterUserAccount(const QByteArray& imageBytes, const QString& userName, const QString& userPassword)
{
	{
		int account = 0;
		while (1) {
			account = QRandomGenerator::global()->bounded(111111111, 999999999);
			int ret = this->isExistTheSameUserAccount(account);
			if (!ret)
				break;
		}
		QSqlDatabase db = QSqlDatabase::database(this->connectName);
		QSqlQuery query(db);
		query.prepare("INSERT INTO UserInfo (userName,userHeadByte,userAccount,userPassword,onlineStatus) VALUES(:username,:userimagebytes,:useraccount,:userpassword,:status)");
		query.bindValue(":username", userName);
		query.bindValue(":userimagebytes", imageBytes);
		query.bindValue(":useraccount", account);
		query.bindValue(":userpassword", userPassword);
		query.bindValue(":status", 0);
		if (query.exec()) {
			emit this->RegisterSucceedSignal(account);
		}
		else
			qInfo() << __FUNCTION__ << __TIME__ << query.lastError();
	}
}

void  UserDatabaseManager::selectUserDataForSearch(const QString& userAccount)
{
	if (userAccount == QString::number(GLOB_UserAccount))
		return;
	SearchFriendData data;
	{
		QSqlDatabase db = QSqlDatabase::database(this->connectName);
		QSqlQuery query(db);
		query.prepare("SELECT userName,userHeadByte,onlineStatus FROM UserInfo WHERE userAccount = " + userAccount);
		if (query.exec()) {
			while (query.next()) {
				QString username = query.value(0).toString();
				QByteArray imagebytes = query.value(1).toByteArray();
				bool status = query.value(2).toBool();
				data.imagebytes = imagebytes;
				data.status = status;
				data.userAccount = userAccount;
				data.userName = username;
			}
		}
	}
	emit SearchFriendDataSignal(data);
}

bool UserDatabaseManager::isExistTheSameUserApplication(const QString& receiver)
{
	//查询是否已存在好友申请
	QSqlDatabase db = QSqlDatabase::database(this->connectName);
	QSqlQuery query(db);
	query.prepare("SELECT receiverAccount From CronyApplicationTemp WHERE senderAccount = " + QString::number(GLOB_UserAccount));
	if (query.exec()) {
		while (query.next()) {
			QString _account = query.value(0).toString();
			if (_account == receiver)
				return true;
		}
	}
	return false;
}

void UserDatabaseManager::increaseUserApplicationTemp(const QString& receiver)
{
	//判断是否存在此好友
	bool is_exist = this->isExistTheUser(receiver);
	if (is_exist) {
		emit this->existTheUserSignal();
		return;
	}

	bool  ret = this->isExistTheSameUserApplication(receiver);
	if (!ret) {
		QSqlDatabase db = QSqlDatabase::database(this->connectName);
		QSqlQuery query(db);
		query.prepare("INSERT INTO CronyApplicationTemp (senderAccount,receiverAccount) VALUES (:sender,:receiver);");
		query.bindValue(":sender", GLOB_UserAccount);
		query.bindValue(":receiver", receiver.toInt());
		if (query.exec()) {
			emit this->isSendApplication(true);
			emit this->SendApplicationToServer(receiver);
		}
		else {
			qDebug() << __FUNCTION__ << __TIME__ << query.lastError();
		}
	}
	else {
		emit this->isSendApplication(false);
	}
}

bool UserDatabaseManager::isExistTheUser(const QString& userAccount)
{
	QSqlDatabase db = QSqlDatabase::database(this->connectName);
	QSqlQuery query(db);
	query.prepare("SELECT cronyAccount FROM UserCrony WHERE userAccount = " + QString::number(GLOB_UserAccount));
	if (query.exec()) {
		while (query.next()) {
			QString account = query.value(0).toString();
			if (account == userAccount)
				return true;
		}
	}
	return false;
}

FriendListData UserDatabaseManager::GetUserFriendData(const QString& cronyAccount)
{
	FriendListData data;
	QSqlDatabase db = QSqlDatabase::database(this->connectName);
	QSqlQuery query(db);
	query.prepare("SELECT userName,userHeadByte,onlineStatus FROM UserInfo WHERE userAccount = " + cronyAccount);
	if (query.exec()) {
		while (query.next()) {
			QString username = query.value(0).toString();
			QByteArray imagebytes = query.value(1).toByteArray();
			bool status = query.value(2).toBool();
			data.status = status;
			data.userName = username;
			data.userAccount = cronyAccount;
			QPixmap pixmap = RoundImage::RoundImageFromByteArray(imagebytes);
			data.userHead = pixmap;
			if (status) {
				pixmap.load(":/Resource/ico/TwemojiGreenCircle.png");
				data.status_text = "在线";
			}

			else {
				data.status_text = "离线";
				pixmap.load(":/Resource/ico/TwemojiRedCircle.png");
			}
			data.status_ico = pixmap;
			return data;
		}
	}
	else {
		qDebug() << query.lastError();
	}
	return FriendListData();
}

void UserDatabaseManager::selectCurrentUserFriends()
{
	if (this->connectName != tr("connect_main"))
		return;
	QList<FriendListData> datas;
	QSqlDatabase db = QSqlDatabase::database(this->connectName);
	QSqlQuery query(db);
	FriendListData data;
	query.prepare("SELECT cronyAccount FROM UserCrony WHERE userAccount = " + QString::number(GLOB_UserAccount));
	if (query.exec()) {
		while (query.next()) {
			QString account = query.value(0).toString();
			data = this->GetUserFriendData(account);
			datas.append(data);
		}
	}
	emit this->userFriends(datas);
}

void UserDatabaseManager::iniSql()
{
	qDebug() << "数据库子线程" << QThread::currentThreadId();
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", this->connectName);
	db.setUserName("root");
	db.setPassword("y..750242");
	db.setPort(3306);
	db.setDatabaseName("users");
	db.setHostName("120.46.157.203");
	if (db.open()) {
		qInfo() << __FUNCTION__ << __TIME__ << "打开数据库成功";
		GLOB_IsConnectedMysql = true;
	}

	else {
		qDebug() << __FUNCTION__ << __TIME__ << db.lastError();
		return;
	}

	this->selectCurrentUserFriends();
}

void UserDatabaseManager::selectUserHeadData(const QString& userAccount)
{
	if (userAccount.isEmpty() || userAccount.length() <= 8)
		return;
	{
		QSqlDatabase db = QSqlDatabase::database(this->connectName);
		QSqlQuery query(db);
		query.prepare("SELECT UserInfo.userHeadByte FROM UserInfo WHERE UserInfo.userAccount =" + userAccount);
		if (query.exec()) {
			while (query.next()) {
				QByteArray imagebytes = query.value(0).toByteArray();
				QPixmap pixmap = RoundImage::RoundImageFromByteArray(imagebytes);
				this->pixmap_temp = pixmap;
				if (!imagebytes.isEmpty())
					emit this->userHeadByteArray(pixmap);
			}
		}
		else {
			qDebug() << query.lastError();
		}
	}
}