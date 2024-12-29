#include "../include/OrnamentServer.h"

OrnamentServer::OrnamentServer(QObject* parent)
	: QObject(parent)
{
	this->tcpServer = new  TcpServer(this);
	this->file_tcpServer = new FileTcpServer(this);
	connect(this->tcpServer, &TcpServer::newClientConnected, this, &OrnamentServer::newClientConnection, Qt::DirectConnection);
	connect(this->file_tcpServer, &FileTcpServer::newFileClientWork, this, &OrnamentServer::newFileServiceConnection, Qt::DirectConnection);
}

OrnamentServer::~OrnamentServer()
{
	foreach(QThread * thread, this->clients.keys()) {
		this->clients.value(thread)->deleteLater();
		thread->quit();
		thread->wait();
		thread->deleteLater();
	}
	this->clients.clear();
}

void OrnamentServer::newClientConnection(const qintptr handle)
{
	QThread* thread = new QThread;
	ClientWork* work = new ClientWork(handle);
	work->moveToThread(thread);
	this->clients.insert(thread, work);

	connect(thread, &QThread::started, work, &ClientWork::initializeSocket, Qt::DirectConnection);
	thread->start();
	connect(work, &ClientWork::clientDisconnected, this, &OrnamentServer::dealClientDisconnecioned, Qt::QueuedConnection);
	connect(work, &ClientWork::newClientLogin, this, &OrnamentServer::dealNewClientLogin, Qt::QueuedConnection);
	connect(this, &OrnamentServer::transferNewClientLogin, work, &ClientWork::NoticeClientLoginSignal, Qt::QueuedConnection);
	connect(work, &ClientWork::AcceptClientApplicationSignal, this, &OrnamentServer::dealClientApplication, Qt::QueuedConnection);
	connect(this, &OrnamentServer::transferClientApplication, work, &ClientWork::accpetClientApplication, Qt::QueuedConnection);
	connect(work, &ClientWork::NoticeSenderUserUpdateFriendListSignal, this, &OrnamentServer::dealNoticeSenderUserUpdateFriendListSignal, Qt::QueuedConnection);
	connect(this, &OrnamentServer::transferNoticeSenderUserUpdateFriendListSignal, work, &ClientWork::sendAddNewUserSucceedNotice, Qt::QueuedConnection);
	connect(work, &ClientWork::acceptUserNorlmalMessage, this, &OrnamentServer::dealAcceptUserNormalMesasge, Qt::QueuedConnection);
	connect(this, &OrnamentServer::transferAcceptUserNormalMessage, work, &ClientWork::senderUserMessageToReceiver, Qt::QueuedConnection);
	connect(work, &ClientWork::NoticeClientDisconnected, this, &OrnamentServer::dealNoticeDisconnected, Qt::QueuedConnection);
	connect(this, &OrnamentServer::transferNoticeDisconnected, work, &ClientWork::dealNoticeClientDisconnected, Qt::QueuedConnection);
}

void OrnamentServer::newFileServiceConnection(const qintptr handle)
{
	QThread* filework_thread = new QThread;
	FileWork* file_work = new FileWork(handle);
	file_work->moveToThread(filework_thread);
	filework_thread->start();
	fileServices.insert(filework_thread, file_work);

	connect(filework_thread, &QThread::started, file_work, &FileWork::initializeFileSocket, Qt::DirectConnection);
	connect(file_work, &FileWork::fileServiceDisconnectedSignal, this, &OrnamentServer::dealServiceDisconnection, Qt::QueuedConnection);
	connect(file_work, &FileWork::transferFileToReceiverSignal, this, &OrnamentServer::dealSendFileForServer, Qt::QueuedConnection);
	//connect(file_work, &FileWork::completeTask, this, &OrnamentServer::dealFileTasks, Qt::QueuedConnection);
	//connect(this, &OrnamentServer::transferSendFileForServer, file_work, &FileWork::SendFileInfo, Qt::QueuedConnection);
}

void OrnamentServer::dealNewClientLogin(const QString& userAccount)
{
	emit this->transferNewClientLogin(userAccount);
}

void OrnamentServer::dealClientDisconnecioned(int socket_id)
{
	for (QThread* thread : this->clients.keys()) {
		this->clients.value(thread)->socketId();
		if (this->clients.value(thread)->socketId() == socket_id) {
			this->clients.value(thread)->deleteLater();

			thread->requestInterruption();
			thread->exit();
			thread->wait();
			this->clients.remove(thread);
			thread->deleteLater();
		}
	}
}

void OrnamentServer::dealClientApplication(const QString& sender, const QString& receiver)
{
	emit transferClientApplication(sender, receiver);
}

void OrnamentServer::dealNoticeSenderUserUpdateFriendListSignal(const QString& senderAccount)
{
	emit this->transferNoticeSenderUserUpdateFriendListSignal(senderAccount);
}

void OrnamentServer::dealAcceptUserNormalMesasge(const QString& senderUserAccount, const QString& receiverUserAccount, const QString& message)
{
	for (QThread* thread : this->clients.keys()) {
		if (receiverUserAccount == this->clients.value(thread)->currentUserAccount()) {
			QMetaObject::invokeMethod(this->clients.value(thread), "senderUserMessageToReceiver", Qt::QueuedConnection, Q_ARG(QString, senderUserAccount), Q_ARG(QString, message));
			break;
		}
	}
}

void OrnamentServer::dealNoticeDisconnected(const QString& userAccount)
{
	emit this->transferNoticeDisconnected(userAccount);
}

void OrnamentServer::dealFileTasks()
{
}

void OrnamentServer::dealSendFileForServer(const QString& senderAccount, const QString& receiveAccount, const QString& filePath)
{
	for (QThread* thread : this->fileServices.keys()) {
		if (receiveAccount == this->fileServices.value(thread)->currentUserAccount()) {
			QMetaObject::invokeMethod(this->fileServices.value(thread), "SendFileInfo", Qt::QueuedConnection, Q_ARG(QString, senderAccount), Q_ARG(QString, filePath));
			return;
		}
	}
}

void OrnamentServer::dealServiceDisconnection(const QString& socket_id)
{
	for (QThread* thread : this->fileServices.keys()) {
		if (this->fileServices.value(thread)->currentSocketId() == socket_id) {
			this->fileServices.value(thread)->deleteLater();

			thread->exit(0);
			thread->wait();
			this->fileServices.remove(thread);
			thread->deleteLater();
		}
	}
	qDebug() << this->fileServices.count();
}