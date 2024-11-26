#include "../include/ChatNetworkManager.h"

ChatNetworkManager::ChatNetworkManager(QObject* parent)
	: QObject(parent)
{
}

ChatNetworkManager::~ChatNetworkManager()
{
	qDebug() << "chat thread delete";
}

void ChatNetworkManager::initializeSocket()
{
	qDebug() << "消息线程" << QThread::currentThreadId();

	this->socket = new QTcpSocket(this);
	//this->socket->connectToHost(QHostAddress("120.46.157.203"), quint16(7502));
	this->socket->connectToHost(QHostAddress("127.0.0.1"), quint16(7502));
	if (!this->socket->waitForConnected()) {
		emit this->connecterrorSignal();
		qDebug() << this->socket->errorString();
		return;
	}
	qDebug() << "连接成功";
	GLOB_IsConnectedServer = true;
	emit this->connectedSignal();

	this->sendLoginInfo();
}

void ChatNetworkManager::sendApplication(const QString& receiver)
{
	QByteArray  out;
	QDataStream stream(&out, QIODevice::WriteOnly);
	int type = MSGTYPE::CronyApplication;
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