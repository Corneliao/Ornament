#pragma once

#include <QPixmap>
#include <QString>
#include  <QListWidgetItem>
#define DEBUGINFO qDebug() << __FILE__ << __FUNCTION__ << __LINE__
extern qreal GLOB_ScaleDpi;
extern bool GLOB_IsConnectedMysql;
extern bool GLOB_IsConnectedServer;
extern int GLOB_UserAccount;
extern QString GLOB_UserName;
extern QSize screenSize;
extern QByteArray GLOB_UserHeadImagebytes;
extern QString SenderFileUserAccountTemp;
extern QString ReceiverFileUserAccountTemp;
extern qreal OneByteForMB;
extern qreal OneByteForKB;

enum  NoticeType { UserApplication };
enum USERDATATYPE { UserApplicationData = 0, UpdateUserFriend };

enum ChatMessageType { TEXT, USERFILE };
enum FILETYPE { EXE, MUSIC, PHOTO };

struct FeatureBarData {
	QPixmap normal_ico;
	QPixmap select_ico;
	QString ico_text;
};

struct FileInfoData {
	QPixmap fileIco;
	QString fileName;
	QString fileSize;
	qreal position = 0.0;
	QString filePath;
	bool isDownloading = false;
	bool isUploading = false;
	int FileType = FILETYPE::EXE;
};

struct UserData {
	QPixmap userHead;
	QString userName;
	QString userAccount;
	QString receiverUserAccount;
	bool status = false;
	QPixmap status_ico;
	QString status_text;
	QString userMessage;
	int index = 0;
	Qt::Alignment alignment = Qt::AlignLeft;
	int messageType = ChatMessageType::TEXT;
	FileInfoData fileInfo;
	bool isUnread = false;
	int unReadMessageNums = 0;
};

struct SearchFriendData {
	QByteArray imagebytes;
	QString userName;
	bool status;
	QString userAccount;
};

struct FILETASKQUEUE {
	QString senderUser;
	QString receiverUser;
	FileInfoData fileInfo;
};

struct FILEQUEUEITEMDATA {
	QPixmap m_file_ico;
	QString m_fileName;
	QString m_fileSize;
	FileInfoData FileInfo;
};
Q_DECLARE_METATYPE(FileInfoData);
