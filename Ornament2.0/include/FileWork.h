#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QMutex>
#include <QThread>
#include <QQueue>
#include "../global.h"
class FileWork : public QObject
{
	Q_OBJECT
		enum MSGTYPE { UserInfo = 0, FileInfo, FileData };
public:
	FileWork(QObject* parent = Q_NULLPTR);
	~FileWork();

public:
	void initializeFileSocket();
	bool currentUploadStatus()const;
	bool currentReceiveStatus()const;

public slots:
	void SendFileInfo(const QString& senderUserAccount, const QString& receiverUserAccount, const FileInfoData& file_data);
private:
	void ReadData();
	void ReceiveFileInfo(const QString& fileName);
	void ReceiveFileData();
	void SendFileData();
	void initFileInfoProperties();
	void dealFileTaskQueue();
private:
	QTcpSocket* m_fileSocket = Q_NULLPTR;
	QString m_filePath_temp;
	qint64 m_fileTotalSize_temp = 0;
	QFile localFile;
	bool  isUploadingFile = false;
	QString fileName;
	QFile newFile;
	bool isReceiveingFile = false;
	qint64 alreadyReceivedBytes = 0;
	qreal m_position = 0.0;
	QQueue<FILETASKQUEUE> FILETASKS;
Q_SIGNALS:
	void updateUploadFileProgressSignal(const qreal& pos);
	void updateDownloadFileProgressSignal(const qreal& pos);
	void ReceiveFileSignal(const QString& senderAccount, const QString& fileName, const qint64& fileSize);
	void completeReceivedFile();
	void finishedForImage(const QString& senderAccount, const QString& fileName, const QString& fileSize);
	void addFileTaskSignal(const QString& fileName, const QString& fileSize);
};
