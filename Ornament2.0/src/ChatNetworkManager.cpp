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
	this->socket = new QTcpSocket(this);
	//this->socket->connectToHost(QHostAddress("120.46.157.203"), quint16(7502));
	this->socket->connectToHost(QHostAddress("127.0.0.1"), quint16(7502));
	if (!this->socket->waitForConnected()) {
		emit this->connecterrorSignal();
		qDebug() << this->socket->errorString();
		return;
	}
	else {
		GLOB_IsConnectedServer = true;
		connect(this->socket, &QTcpSocket::readyRead, this, &ChatNetworkManager::ReadData, Qt::DirectConnection);
		emit this->connectedSignal();
		this->sendLoginInfo();
	}
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
	else if (type == MSGTYPE::ReceivedFileInfo) {
		this->SendFileDataToServer();
	}
	else if (type == MSGTYPE::SendFileInfo) {
		QString senderAccount;
		QString fileName;
		qint64 fileSize;
		stream >> senderAccount >> fileName >> fileSize;
		SenderFileUserAccountTemp = senderAccount;
		emit this->ReceiveFileForServertSignal(senderAccount, fileName, fileSize);
		qDebug() << "我是" << GLOB_UserName << "收到：" << senderAccount << "文件" << fileName << fileSize;
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

void ChatNetworkManager::SendUserFile(const QString& senderUserAccount, const QString& receiverUserAccount, const FileInfoData& file_data)
{
	this->filePath = file_data.filePath;
	this->fileTotalSize = file_data.fileSize.toLongLong();
	qDebug() << __FUNCTION__ << __TIME__ << "获取到文件信息 ：" << "文件名：" << file_data.fileName << "文件大小：" << file_data.fileSize << "文件路径：" << file_data.filePath;
	QByteArray out;
	QDataStream stream(&out, QIODevice::WriteOnly);
	int type = MSGTYPE::SendFileInfo;
	stream << type << senderUserAccount << receiverUserAccount << QString(file_data.fileName) << file_data.fileSize.toLongLong();
	this->socket->write(out);
}

void ChatNetworkManager::SendFileDataToServer()
{
	QFile file(this->filePath);
	if (!file.open(QFile::ReadOnly))
		return;
	qDebug() << __LINE__ << __FUNCTION__ << "文件打开成功";
	qint64 loadSize = static_cast<qint64>(64) * 1024;
	qint64 alreadySendByteSize = 0;
	qreal position = 0.0;
	while (alreadySendByteSize != this->fileTotalSize)
	{
		if (alreadySendByteSize < this->fileTotalSize)
		{
			position = static_cast<qreal>(alreadySendByteSize * 1.0 / this->fileTotalSize);
			emit this->updateUploadFileProgress(position);
			QByteArray fileData = file.read(qMin(loadSize, this->fileTotalSize));
			qint64 socketSize = this->socket->write(fileData, fileData.size());
			if (this->socket->waitForBytesWritten())
				alreadySendByteSize += socketSize;
			else
				return;
		}
		if (alreadySendByteSize == this->fileTotalSize) {
			position = static_cast<qreal>(alreadySendByteSize * 1.0 / this->fileTotalSize);
			emit this->updateUploadFileProgress(position);
			this->fileTotalSize = 0;
			file.close();

			return;
		}
	}
}
