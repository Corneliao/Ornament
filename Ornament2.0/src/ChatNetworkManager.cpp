#include "../include/ChatNetworkManager.h"

ChatNetworkManager::ChatNetworkManager(QObject* parent)
	: QObject(parent)
{
}

ChatNetworkManager::~ChatNetworkManager()
{
	DEBUGINFO << "客户端线程销毁" << QThread::currentThreadId();
}

void ChatNetworkManager::initializeSocket()
{
	DEBUGINFO << "客户端线程：" << QThread::currentThreadId();
	this->socket = new QTcpSocket(this);
	//this->socket->connectToHost(QHostAddress("120.46.157.203"), quint16(7502));
	this->socket->connectToHost(QHostAddress("127.0.0.1"), quint16(7502));

	if (!this->socket->waitForConnected()) {
		emit this->connecterrorSignal();
		qDebug() << this->socket->errorString();
		return;
	}


	GLOB_IsConnectedServer = true;
	connect(this->socket, &QTcpSocket::readyRead, this, &ChatNetworkManager::ReadData, Qt::DirectConnection);
	emit this->connectedSignal();
	this->sendLoginInfo();
}

void ChatNetworkManager::sendApplication(const QString& receiver)
{
	QByteArray  out;
	QDataStream stream(&out, QIODevice::WriteOnly);
	int type = MSGTYPE::FriendApplication;
	stream << type << QString::number(GLOB_UserAccount) << receiver;
	this->socket->write(out);
	if (!this->socket->waitForBytesWritten())
		qDebug() << this->socket->errorString();
}

void ChatNetworkManager::sendLoginInfo()
{
	QByteArray out;
	QDataStream stream(&out, QIODevice::WriteOnly);
	int type = MSGTYPE::Login;
	stream << type << QString::number(GLOB_UserAccount);
	this->socket->write(out);;
	if (!this->socket->waitForBytesWritten())
		qDebug() << this->socket->errorString();
}

void ChatNetworkManager::ReadData()
{
	QDataStream stream(this->socket);
	int type;
	stream >> type;
	if (type == MSGTYPE::NoticeNewLogin) {
		QString account;
		stream >> account;
		emit this->UserLogined(account);
	}
	else if (type == MSGTYPE::WaitAcceptApplication) {
		QString sender;
		stream >> sender;
		emit this->acceptUserApplication(sender, USERDATATYPE::UserApplicationData);
	}
	else if (type == MSGTYPE::AcceptedApplication) {
		QString receiverAccount;
		stream >> receiverAccount;
		emit this->updateUserFriendList(receiverAccount, USERDATATYPE::UpdateUserFriend);
	}
	else if (type == MSGTYPE::SendNormalMessage) {
		QString senderUserAccount;
		QString message;
		stream >> senderUserAccount >> message;
		emit this->acceptUserNormalMessage(senderUserAccount, message);
	}
	else if (type == MSGTYPE::SendUserDisconnected) {
		QString userAccount;
		stream >> userAccount;
		emit this->userDisconnectedSignal(userAccount);
	}
}

void ChatNetworkManager::acceptApplication(const QString& userAccount)
{
	QByteArray out;
	QDataStream stream(&out, QIODevice::WriteOnly);
	int type = MSGTYPE::SendAcceptApplicationNotice;
	stream << type << userAccount;
	this->socket->write(out);
}

void ChatNetworkManager::sendUserNormalMessage(const QString& senderUserAccount, const QString& receiverUserAccount, const QString& message)
{
	QByteArray out;
	QDataStream stream(&out, QIODevice::WriteOnly);
	int type = MSGTYPE::NormalMessage;
	stream << type << senderUserAccount << receiverUserAccount << message;
	this->socket->write(out);
}
