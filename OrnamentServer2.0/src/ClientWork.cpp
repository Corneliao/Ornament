#include "../include/ClientWork.h"

ClientWork::ClientWork(const qintptr& handle, QObject* parent)
	: QObject(parent)
{
	this->socketDecriptor = handle;
}

ClientWork::~ClientWork()
{
	qDebug() << __FUNCTION__ << __TIME__ << "客户端销毁：" << QThread::currentThreadId();

	if (this->sql_thread) {
		this->userDatabase->deleteLater();
		this->userDatabase = Q_NULLPTR;

		this->sql_thread->exit(0);
		this->sql_thread->wait();
		this->sql_thread->deleteLater();
		this->sql_thread = Q_NULLPTR;
	}
}

void ClientWork::initializeSocket()
{
	qDebug() << __FUNCTION__ << __TIME__ << "客户端线程：" << QThread::currentThreadId();
	this->socket_id = QString::number(quintptr(QThread::currentThreadId())).toInt();
	qDebug() << this->socket_id;
	this->socket = new QTcpSocket(this);
	this->socket->setSocketDescriptor(this->socketDecriptor);
	qDebug() << this->socket->peerAddress() << this->socket->peerPort();
	connect(this->socket, &QTcpSocket::readyRead, this, &ClientWork::ReadData, Qt::DirectConnection);
	connect(this->socket, &QTcpSocket::disconnected, this, &ClientWork::dealClientDisonnected, Qt::DirectConnection);

	//初始化数据库线程
	this->sql_thread = new QThread;
	this->userDatabase = new UserDatabaseManager;
	this->userDatabase->moveToThread(this->sql_thread);
	connect(this->sql_thread, &QThread::started, this->userDatabase, &UserDatabaseManager::initialSql, Qt::DirectConnection);
	this->sql_thread->start();
	connect(this, &ClientWork::updateClientStatus, this->userDatabase, &UserDatabaseManager::updateClientStatus, Qt::QueuedConnection);
	connect(this, &ClientWork::NoticeClientLoginSignal, this->userDatabase, &UserDatabaseManager::isExistTheUser, Qt::QueuedConnection);
	connect(this->userDatabase, &UserDatabaseManager::isEixstTheUserSignal, this, &ClientWork::noticeUserLogin, Qt::QueuedConnection);
	connect(this->userDatabase, &UserDatabaseManager::addNewUserSucceed, this, &ClientWork::sendAddNewUserSucceedNotice, Qt::QueuedConnection);
	connect(this, &ClientWork::FindFreindForDisconnected, this->userDatabase, &UserDatabaseManager::FindFriendForDisconnected, Qt::QueuedConnection);
	connect(this->userDatabase, &UserDatabaseManager::FindFriendForDisconnectedSignal, this, &ClientWork::NoticeClientDisconnectedToUser, Qt::QueuedConnection);
}

int ClientWork::socketId() const
{
	return this->socket_id;
}

void ClientWork::noticeUserLogin(const QString& useraccount, bool isExist)
{
	//判断是否为用户好友 如果存在则向用户发送好友登录消息
	if (!isExist)
		return;
	QMutexLocker locker(&mutex);
	QByteArray out;
	QDataStream stream(&out, QIODevice::WriteOnly);
	int type = MSGTYPE::NoticeNewLogin;
	stream << type << useraccount;
	this->socket->write(out);
}

void ClientWork::accpetClientApplication(const QString& sender, const QString& receiver)
{
	if (this->userAccount == receiver) {
		qDebug() << __TIME__ << receiver << "收到" << sender << "的好友申请 等待客户端确认是否同意";
		QByteArray out;
		QDataStream stream(&out, QIODevice::WriteOnly);
		int type = MSGTYPE::WaitAcceptApplication;
		stream << type << sender;
		this->socket->write(out);
	}
}

void ClientWork::sendAddNewUserSucceedNotice(const QString& senderAccount)
{
	if (this->userAccount != senderAccount)
		return;
	QByteArray out;
	QDataStream stream(&out, QIODevice::WriteOnly);
	int type = MSGTYPE::AcceptedApplication;
	stream << type << this->receiverUserAccountTemp;
	this->socket->write(out);
}

void ClientWork::senderUserMessageToReceiver(const QString& senderUserAccount, const QString& message)
{
	//QMutexLocker locker(&mutex);
	QByteArray out;
	QDataStream stream(&out, QIODevice::WriteOnly);
	int type = MSGTYPE::SendNormalMesssage;
	stream << type << senderUserAccount << message;
	this->socket->write(out);
	qDebug() << "发送zhe" << senderUserAccount;
}

void ClientWork::dealNoticeClientDisconnected(const QString& user_account)
{
	if (user_account == this->userAccount)
		return;
	emit this->FindFreindForDisconnected(user_account);
}

void ClientWork::NoticeClientDisconnectedToUser(const QString& userAccount, bool isExist)
{
	if (!isExist)
		return;
	QMutexLocker locker(&mutex);
	QByteArray out;
	QDataStream stream(&out, QIODevice::WriteOnly);
	int type = MSGTYPE::SendUserDisconnected;
	stream << type << userAccount;
	this->socket->write(out);
}

QString ClientWork::currentUserAccount() const
{
	return this->userAccount;
}

void ClientWork::ReadData()
{
	QDataStream stream(this->socket);
	int type;
	stream >> type;

	if (type == MSGTYPE::Login) {
		QString account;
		stream >> account;
		this->userAccount = account;
		emit this->updateClientStatus(this->userAccount, "1");
		emit this->newClientLogin(this->userAccount);
	}
	else if (type == MSGTYPE::FriendApplication) {
		QString sender;
		QString receiver;
		stream >> sender >> receiver;
		this->receiverUserAccountTemp = receiver;
		emit this->AcceptClientApplicationSignal(sender, receiver);
	}
	else if (type == MSGTYPE::SendAcceptApplicationNotice) {
		QString senderAccount;
		stream >> senderAccount;
		emit this->NoticeSenderUserUpdateFriendListSignal(senderAccount);
	}
	else  if (type == MSGTYPE::NormalMessage) {
		QString senderUserAccount;
		QString receiverUserAccount;
		QString message;
		stream >> senderUserAccount >> receiverUserAccount >> message;
		emit this->acceptUserNorlmalMessage(senderUserAccount, receiverUserAccount, message);
	}
}

void ClientWork::dealClientDisonnected()
{
	emit this->NoticeClientDisconnected(this->userAccount);
	emit this->updateClientStatus(this->userAccount, "0");

	emit this->clientDisconnected(this->socket_id);
}