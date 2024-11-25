#include "../include/UserDatabaseManager.h"

UserDatabaseManager::UserDatabaseManager(const QString& connect_name)
{
	this->connectName = connect_name;
}

UserDatabaseManager::~UserDatabaseManager()
{
	qDebug() << "do work deleter";
}

void UserDatabaseManager::closeDatabase()
{
	{
		QSqlDatabase db = QSqlDatabase::database(this->connectName);
		db.close();
	}
	QSqlDatabase::removeDatabase(this->connectName);
	emit this->closedDatabaseSignal();
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
								emit this->VerifyFailed(true);
							return;
						}
					}
				}
			}
			emit this->UnValidUserAccount(true);
		}
		else {
			qDebug() << "语句错误";
		}
	}
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
	if (db.open())
		GLOB_IsConnectedMysql = true;

	else {
		qDebug() << db.lastError();
		return;
	}
}

void UserDatabaseManager::selectUserData(const QString& userAccount)
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