#include "../include/UserDatabaseManager.h"

UserDatabaseManager::UserDatabaseManager(const QString& connect_name)
{
	this->connectName = connect_name;
	qRegisterMetaType<QList<UserData>>("QList<UserData>");
}

UserDatabaseManager::~UserDatabaseManager()
{
	DEBUGINFO << "数据库线程销毁" << QThread::currentThreadId();
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

/**
 * @brief 注册账号时判断是否已存在账号 存在则随机生成
 * @param account
 * @return
 */
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

/**
 * @brief 注册账号
 * @param imageBytes
 * @param userName
 * @param userPassword
 */
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

/**
 * @brief 获取搜索时用户数据
 * @param userAccount
 */
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

/**
 * @brief 判断是否已存在好友申请
 * @param receiver
 * @return
 */

bool UserDatabaseManager::isExistTheSameUserApplication(const QString& receiver) const
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

/**
 * @brief 添加临时好友申请
 * @param receiver
 */
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
			//判断当前好友是否在线，在线则向服务器发送申请通知
			query.prepare("SELECT onlineStatus FROM UserInfo WHERE userAccount = " + receiver);
			if (query.exec()) {
				while (query.next()) {
					bool status = query.value(0).toBool();
					if (status)
						emit this->SendApplicationToServer(receiver);
					else
						qDebug() << "当前好友未在线,在线好友申请未发送";
				}
			}
		}
		else {
			qDebug() << __FUNCTION__ << __TIME__ << query.lastError();
		}
	}
	else {
		emit this->isSendApplication(false);
	}
}

/**
 * @brief 判断是否已存在此好友
 * @param userAccount
 * @return 存在return true 不存在return false
 */
bool UserDatabaseManager::isExistTheUser(const QString& userAccount) const
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

/**
 * @brief 程序启动时获取用户信息
 * @param cronyAccount
 * @return
 */
UserData UserDatabaseManager::GetUserFriendData(const QString& cronyAccount) const
{
	UserData data;
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
	return UserData();
}

//程序启动时获取好友列表
void UserDatabaseManager::selectCurrentUserFriends()
{
	if (this->connectName != tr("connect_main"))
		return;
	QList<UserData> datas;
	QSqlDatabase db = QSqlDatabase::database(this->connectName);
	QSqlQuery query(db);
	UserData data;
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

/**
 * @brief 获取用户数据
 * @param userAccount
 * @param type 返回用户数据的类型
 */
void UserDatabaseManager::selectUserData(const QString& userAccount, int type)
{
	UserData data;
	QSqlDatabase db = QSqlDatabase::database(this->connectName);
	QSqlQuery query(db);
	query.prepare("SELECT userName,userHeadByte,onlineStatus FROM UserInfo WHERE userAccount = " + userAccount);
	if (query.exec()) {
		while (query.next()) {
			QString username = query.value(0).toString();
			QByteArray imagebytes = query.value(1).toByteArray();
			bool status = query.value(2).toBool();
			data.status = status;
			data.userName = username;
			data.userAccount = userAccount;
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
			if (type == USERDATATYPE::UserApplicationData)
				emit this->userDataSignal(data);
			else if (type == USERDATATYPE::UpdateUserFriend)
				emit this->updateFriendListDataSignal(data);
		}
	}
	else {
		qDebug() << query.lastError();
	}
}

/**
 * @brief 添加好友至数据库
 * @param userAccount
 */
void UserDatabaseManager::addUserFriend(const QString& userAccount)
{
	QSqlDatabase db = QSqlDatabase::database(this->connectName);
	QSqlQuery query(db);
	query.prepare("INSERT INTO UserCrony (userAccount,cronyAccount) VALUES (:user,:crony);");
	query.bindValue(":user", GLOB_UserAccount);
	query.bindValue(":crony", userAccount);
	bool ret = query.exec();

	query.prepare("INSERT INTO UserCrony (userAccount,cronyAccount) VALUES (:user,:crony);");
	query.bindValue(":user", userAccount);
	query.bindValue(":crony", GLOB_UserAccount);
	if (ret && query.exec()) {
		qDebug() << __FUNCTION__ << __TIME__ << "添加好友成功";
		query.prepare("DELETE FROM CronyApplicationTemp WHERE senderAccount = " + userAccount + " AND receiverAccount = " + QString::number(GLOB_UserAccount));
		if (query.exec()) {
			qDebug() << __FUNCTION__ << __TIME__ << "删除申请记录";
		}
		emit AcceptedApplicationSignal(userAccount);
	}
}

/**
 * @brief 初始化数据库
 */
void UserDatabaseManager::iniSql()
{
	DEBUGINFO << "数据库线程：" << QThread::currentThreadId();
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", this->connectName);
	db.setUserName("root");
	db.setPassword("y..750242");
	db.setPort(3306);
	db.setDatabaseName("users");
	db.setHostName("120.46.157.203");
	if (db.open()) {
		GLOB_IsConnectedMysql = true;
	}

	else {
		qDebug() << __FUNCTION__ << __TIME__ << db.lastError();
		QSqlError error = db.lastError();
		//	emit this->openDatabaseFailedSignal(error.text());
		return;
	}

	this->selectCurrentUserFriends();
}

/**
 * @brief 获取用户头像
 * @param userAccount
 */
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
