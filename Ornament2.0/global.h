#pragma once

#include <QPixmap>
#include <QString>
#include  <QListWidgetItem>
extern qreal GLOB_ScaleDpi;
extern bool GLOB_IsConnectedMysql;
extern bool GLOB_IsConnectedServer;
extern int GLOB_UserAccount;
extern QString GLOB_UserName;
extern QSize screenSize;
extern QByteArray GLOB_UserHeadImagebytes;
//extern QMap<QString, QListWidgetItem*> FileItemQueue;
extern QString SenderFileUserAccountTemp;
extern QString ReceiverFileUserAccountTemp;
//extern QPixmap GLOB_UserHead;

enum  NoticeType { UserApplication };
enum USERDATATYPE { UserApplicationData = 0, UpdateUserFriend };

enum ChatMessageType { TEXT, USERFILE };
enum FILETYPE { EXE, MUSIC };
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
	bool status = false;
	QPixmap status_ico;
	QString status_text;
	QString userMessage;
	int index = 0;
	Qt::Alignment alignment = Qt::AlignLeft;
	int messageType = ChatMessageType::TEXT;
	FileInfoData fileInfo;
};

//struct FriendChatData {
//	QPixmap userHead;
//	QString userName;
//	QString userMessage;
//	bool status = false;
//};
struct SearchFriendData {
	QByteArray imagebytes;
	QString userName;
	bool status;
	QString userAccount;
};

//struct FriendListData {
//	QPixmap userHead;
//	QString userName;
//	bool status = false;
//	QString userAccount;
//	QPixmap status_ico;
//	QString status_text;
//};
