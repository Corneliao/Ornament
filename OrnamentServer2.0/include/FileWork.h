#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QMutex>
#include <QMutexLocker>
#include  <QTimer>
#include  "global.h"

#define DEBUG qDebug() << __FUNCTION__ << __TIME__
class FileWork : public QObject
{
	Q_OBJECT
		enum MSGTYPE { UserInfo = 0, FileInfo, FileData };
public:
	FileWork(const qintptr handle, QObject* parent = Q_NULLPTR);
	~FileWork();
public:
	void initializeFileSocket();
	Q_INVOKABLE	void SendFileInfo(const QString& senderAccount, const QString& filePath);
	QString currentSocketId()const;
	QString currentUserAccount()const;
	bool currentStatus()const;
	void dealFileTasks();
private:
	void ReadData();
	void ReceiveFileInfo(const QString& fileName);
	void ReceiveFileData();
	void SendFileData();
	void initFileInfoProperties();
	void FileServiceDisconnected();
	QString socket_id;
private:
	QTcpSocket* m_fileSocket = Q_NULLPTR;
	qintptr m_handle = 0;
	qint64 m_fileTotleSize = 0;
	bool isReceivingFile = false; //客户端正在上传文件至服务器
	QFile newFile;
	qint64 alreadyReceivedByteSize = 0;
	qreal m_position = 0.0;
	QString m_filepath;
	QString sender_temp;
	QString receiver_temp;
	QString m_userAccount;
	bool isSendingFile = false;
	QFile localFile;
	//bool isSendingFile = 0; //客户端线程正在向客户端发送文件
	QMutex m_mutex;
	bool isTaskingFile = false;
	QQueue<FILETASKQUEUE> FILETASKS;
Q_SIGNALS:
	void transferFileToReceiverSignal(const QString& sender, const QString& receiver, const QString& filepath);
	void fileServiceDisconnectedSignal(const QString& socket_id);
	void completeTask();
};
