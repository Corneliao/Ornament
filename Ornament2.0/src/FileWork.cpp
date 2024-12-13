#include "../include/FileWork.h"

FileWork::FileWork(QObject* parent)
	: QObject(parent)
{
	qRegisterMetaType<FileInfoData>("FileInfoData&");
}

FileWork::~FileWork()
{
	DEBUGINFO << "文件传输线程销毁" << QThread::currentThreadId();
}

void FileWork::initializeFileSocket()
{
	DEBUGINFO << "文件传输线程：" << QThread::currentThreadId();
	this->m_fileSocket = new  QTcpSocket(this);
	this->m_fileSocket->setSocketOption(QAbstractSocket::KeepAliveOption, true);
	//this->m_fileSocket->connectToHost(QHostAddress("120.46.157.203"), quint16(7028));
	this->m_fileSocket->connectToHost(QHostAddress("127.0.0.1"), quint16(7028));
	if (!this->m_fileSocket->waitForConnected()) {
		return;
	}
	DEBUGINFO << "连接文件传输服务成功";
	connect(this->m_fileSocket, &QTcpSocket::readyRead, this, &FileWork::ReadData, Qt::DirectConnection);
	connect(this, &FileWork::completeReceivedFile, this, &FileWork::dealFileTaskQueue, Qt::DirectConnection);
	QByteArray out;
	QDataStream stream(&out, QIODevice::WriteOnly);
	int type = MSGTYPE::UserInfo;
	stream << type << QString::number(GLOB_UserAccount);
	this->m_fileSocket->write(out);
}

void FileWork::SendFileInfo(const QString& senderUserAccount, const QString& receiverUserAccount, const FileInfoData& file_data)
{
	if (this->isReceiveingFile) {
		qDebug() << "当前正在接受文件,已加入队列";
		FILETASKQUEUE task;
		task.senderUser = senderUserAccount;
		task.receiverUser = receiverUserAccount;
		task.fileInfo = file_data;
		this->FILETASKS.enqueue(task);
		return;
	}
	QMutexLocker locker(&this->m_mutex);
	DEBUGINFO << "文件信息：" << file_data.fileName << file_data.fileSize << "发送者：" << senderUserAccount << "接收者：" << receiverUserAccount;
	this->m_filePath_temp = file_data.filePath;
	this->m_fileTotalSize_temp = file_data.fileSize.toLongLong();
	QByteArray out;
	QDataStream stream(&out, QIODevice::WriteOnly);
	int type = MSGTYPE::FileInfo;
	stream << type << senderUserAccount << receiverUserAccount << file_data.fileName << file_data.fileSize;
	this->m_fileSocket->write(out);
}

void FileWork::SendFileData()
{
	this->localFile.setFileName(this->m_filePath_temp);
	if (!this->localFile.open(QFile::ReadOnly)) {
		qDebug() << __FUNCTION__ << "文件打开失败";
		return;
	}
	this->isUploadingFile = true;
	qint64 loadSize = static_cast<qint64>(200) * 1024;
	qint64 alreadyWrittenByteSize = 0;
	qreal position = 0.0;
	while (alreadyWrittenByteSize != this->m_fileTotalSize_temp) {
		if (QThread::currentThread()->isInterruptionRequested())
			return;
		if (alreadyWrittenByteSize < this->m_fileTotalSize_temp) {
			position = alreadyWrittenByteSize * 1.0 / this->m_fileTotalSize_temp;
			emit this->updateUploadFileProgressSignal(position);
			const QByteArray FileData = this->localFile.read(qMin(loadSize, this->m_fileTotalSize_temp));
			const qint64 WrittenSocketSize = this->m_fileSocket->write(FileData, FileData.size());
			if (!this->m_fileSocket->waitForBytesWritten()) {
				this->initFileInfoProperties();
				return;
			}
			alreadyWrittenByteSize += WrittenSocketSize;
			QThread::usleep(1000);
		}

		if (alreadyWrittenByteSize == this->m_fileTotalSize_temp) {
			position = static_cast<qreal>(alreadyWrittenByteSize * 1.0 / this->m_fileTotalSize_temp);
			emit this->updateUploadFileProgressSignal(position);
			this->initFileInfoProperties();
			emit this->completeReceivedFile();
			qDebug() << "发送完成";
			return;
		}
	}
}

bool FileWork::currentUploadStatus() const
{
	return this->isUploadingFile;
}

bool FileWork::currentReceiveStatus()const {
	return this->isReceiveingFile;
}

void FileWork::ReadData()
{
	if (this->isReceiveingFile) {
		this->ReceiveFileData();
		return;
	}
	QDataStream stream(this->m_fileSocket);
	int type;
	stream >> type;
	if (type == MSGTYPE::FileInfo) {
		QString senderUserAccount;
		QString fileName;
		qint64 fileSize;
		stream >> senderUserAccount >> fileName >> fileSize;
		this->m_fileTotalSize_temp = fileSize;
		this->ReceiveFileInfo(fileName);
		DEBUGINFO << "接收到：" << senderUserAccount << "的文件" << fileName << fileSize;
		SenderFileUserAccountTemp = senderUserAccount;
		emit this->ReceiveFileSignal(senderUserAccount, fileName, fileSize);
	}
	else if (type == MSGTYPE::FileData) {
		this->SendFileData();
	}
}

void FileWork::ReceiveFileInfo(const QString& fileName)
{
	this->fileName = fileName;
	this->newFile.setFileName(fileName);
	if (!this->newFile.open(QFile::WriteOnly)) {
		DEBUGINFO << "文件打开失败";
		return;
	}

	QByteArray out;
	QDataStream stream(&out, QIODevice::WriteOnly);
	int type = MSGTYPE::FileData;
	stream << type;
	this->m_fileSocket->write(out);
	this->isReceiveingFile = true;
}

void FileWork::ReceiveFileData()
{
	qint64 bytesAvailable = this->m_fileSocket->bytesAvailable();
	if (bytesAvailable <= 0) {
		this->m_fileTotalSize_temp = 0;
		this->isReceiveingFile = false;
		if (this->newFile.isOpen())
			this->newFile.close();
		return;
	}

	if (alreadyReceivedBytes < this->m_fileTotalSize_temp) {
		this->m_position = this->alreadyReceivedBytes * 1.0 / this->m_fileTotalSize_temp;
		emit this->updateDownloadFileProgressSignal(this->m_position);
		const QByteArray FileData = this->m_fileSocket->read(bytesAvailable);
		const qint64 WrittenFileDataSize = this->newFile.write(FileData, FileData.size());
		this->alreadyReceivedBytes += WrittenFileDataSize;
	}

	if (alreadyReceivedBytes == this->m_fileTotalSize_temp) {
		this->m_position = this->alreadyReceivedBytes * 1.0 / this->m_fileTotalSize_temp;
		emit this->updateDownloadFileProgressSignal(this->m_position);
		qDebug() << "接受完成";
		emit this->finishedForImage(SenderFileUserAccountTemp, this->fileName, QString::number(this->m_fileTotalSize_temp));
		this->newFile.close();
		this->alreadyReceivedBytes = 0;
		this->m_fileTotalSize_temp = 0;
		this->isReceiveingFile = false;
		emit this->completeReceivedFile();

		return;
	}
}

void FileWork::initFileInfoProperties()
{
	this->m_fileTotalSize_temp = 0;
	this->isUploadingFile = false;
	if (this->localFile.isOpen())
		localFile.close();
}

void FileWork::dealFileTaskQueue()
{
	if (this->FILETASKS.count() != 0) {
		qDebug() << "开始处理任务";
		FILETASKQUEUE task = this->FILETASKS.dequeue();
		QString senderUser = task.senderUser;
		QString  receiveUser = task.receiverUser;
		FileInfoData file_info = task.fileInfo;
		this->SendFileInfo(senderUser, receiveUser, file_info);
	}
}