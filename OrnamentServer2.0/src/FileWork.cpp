#include "../include/FileWork.h"

//QMutex FileWork::m_mutex;
FileWork::FileWork(const qintptr handle, QObject* parent)
	: QObject(parent)
{
	this->m_handle = handle;
}

FileWork::~FileWork()
{
	qDebug() << __FUNCTION__ << __TIME__ << "文件传输" << QThread::currentThreadId() << "已被销毁";
}

void FileWork::initializeFileSocket()
{
	this->socket_id = QString::number(quintptr(QThread::currentThreadId()));
	qDebug() << __FUNCTION__ << __LINE__ << "文件传输线程：" << QThread::currentThreadId();
	this->m_fileSocket = new QTcpSocket(this);
	this->m_fileSocket->setSocketDescriptor(this->m_handle);
	connect(this->m_fileSocket, &QTcpSocket::readyRead, this, &FileWork::ReadData, Qt::DirectConnection);
	connect(this->m_fileSocket, &QTcpSocket::disconnected, this, &FileWork::FileServiceDisconnected, Qt::DirectConnection);
	connect(this, &FileWork::completeTask, this, &FileWork::dealFileTasks, Qt::DirectConnection);
}

void FileWork::ReadData()
{
	if (this->isReceivingFile) {
		this->ReceiveFileData();
		return;
	}
	QDataStream stream(this->m_fileSocket);
	int type;
	stream >> type;
	if (type == MSGTYPE::UserInfo)
	{
		QString userAccount;
		stream >> userAccount;
		this->m_userAccount = userAccount;
		qDebug() << this->m_userAccount << "登录";
	}
	if (type == MSGTYPE::FileInfo) {
		QString senderUserAccount;
		QString receiveUserAccount;
		QString fileName;
		QString fileSize;
		stream >> senderUserAccount >> receiveUserAccount >> fileName >> fileSize;
		this->sender_temp = senderUserAccount;
		this->receiver_temp = receiveUserAccount;
		this->m_fileTotleSize = fileSize.toLongLong();
		this->ReceiveFileInfo(fileName);
	}
	else if (type == MSGTYPE::FileData) {
		this->SendFileData();
	}
}

void FileWork::ReceiveFileInfo(const QString& fileName)
{
	QString save_path = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/" + fileName;
	this->m_filepath = save_path;
	this->newFile.setFileName(save_path);
	if (!this->newFile.open(QFile::WriteOnly)) {
		return;
	}
	QByteArray out;
	QDataStream stream(&out, QIODevice::WriteOnly);
	int type = MSGTYPE::FileData;
	stream << type;
	this->m_fileSocket->write(out);
	this->isReceivingFile = true;
}

void FileWork::ReceiveFileData()
{
	qint64 byteSize = this->m_fileSocket->bytesAvailable();
	if (byteSize <= 0) {
		this->initFileInfoProperties();
		return;
	}

	if (QThread::currentThread()->isInterruptionRequested()) {
		this->initFileInfoProperties();
		return;
	}

	if (this->alreadyReceivedByteSize < this->m_fileTotleSize) {
		const QByteArray FileData = this->m_fileSocket->read(byteSize);
		qint64 WrittenFileSize = this->newFile.write(FileData, FileData.size());
		this->alreadyReceivedByteSize += WrittenFileSize;
	}

	if (this->alreadyReceivedByteSize == this->m_fileTotleSize) {
		emit this->transferFileToReceiverSignal(this->sender_temp, this->receiver_temp, this->m_filepath);
		this->initFileInfoProperties();
		qDebug() << "接受完成";
		emit completeTask();
		return;
	}
}

void FileWork::SendFileData()
{
	this->localFile.setFileName(this->m_filepath);
	if (!this->localFile.open(QFile::ReadOnly))
	{
		return;
	}

	const qint64 loadSize = static_cast<qint64>(5) * 1024 * 1024;
	qint64 alreadtSendBytes = 0;
	qreal position = 0.0;
	qDebug() << __FUNCTION__ << __LINE__ << "开始发送文件给客户端";
	while (alreadtSendBytes != this->m_fileTotleSize) {
		if (QThread::currentThread()->isInterruptionRequested()) {
			this->m_fileTotleSize = 0;
			this->localFile.close();
			return;
		}
		if (alreadtSendBytes < this->m_fileTotleSize) {
			const QByteArray FileData = this->localFile.read(qMin(loadSize, this->m_fileTotleSize));
			const qint64 WrittenSocketSize = this->m_fileSocket->write(FileData, FileData.size());
			if (!this->m_fileSocket->waitForBytesWritten()) {
				this->m_fileTotleSize = 0;
				this->localFile.close();
				return;
			}
			alreadtSendBytes += WrittenSocketSize;
			QThread::usleep(1000);
		}
		if (alreadtSendBytes == this->m_fileTotleSize) {
			this->m_fileTotleSize = 0;
			this->localFile.close();
			emit completeTask();
			qDebug() << "完成发送任务";
			return;
		}
	}
}

void FileWork::SendFileInfo(const QString& senderAccount, const QString& filePath)
{
	if (this->isReceivingFile) {
		qDebug() << "当前繁忙，已添加至任务队列";
		FILETASKQUEUE task;
		task.senderUserAccount = senderAccount;
		task.filepath = filePath;
		this->FILETASKS.enqueue(task);
		return;
	}
	this->m_mutex.lock();
	this->m_filepath = filePath;
	QFileInfo info(this->m_filepath);
	QString fileName = info.fileName();
	qint64 fileSize = info.size();
	this->m_fileTotleSize = fileSize;
	QByteArray out;
	QDataStream stream(&out, QIODevice::WriteOnly);
	int type = MSGTYPE::FileInfo;
	stream << type << senderAccount << fileName << fileSize;
	this->m_fileSocket->write(out);
	this->m_mutex.unlock();
}

QString FileWork::currentSocketId() const
{
	return this->socket_id;
}

QString FileWork::currentUserAccount() const
{
	return this->m_userAccount;
}

bool FileWork::currentStatus() const
{
	if (this->isReceivingFile)
		return  this->isReceivingFile;
	else if (isSendingFile)
		return this->isSendingFile;
	return false;
}

void FileWork::dealFileTasks()
{
	if (this->FILETASKS.count() != 0) {
		FILETASKQUEUE task = this->FILETASKS.dequeue();
		QString filePath = task.filepath;
		QString senderUser = task.senderUserAccount;
		this->SendFileInfo(senderUser, filePath);
	}
}

void FileWork::initFileInfoProperties()
{
	this->m_fileTotleSize = 0;
	this->alreadyReceivedByteSize = 0;
	this->isReceivingFile = false;
	if (this->newFile.isOpen())  
		this->newFile.close();
}

void FileWork::FileServiceDisconnected()
{
	emit this->fileServiceDisconnectedSignal(this->socket_id);
}