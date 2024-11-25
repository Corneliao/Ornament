#include "../include/FriendChatList.h"

FriendChatList::FriendChatList(QWidget* parent)
	: QWidget(parent)
{
	this->setFixedWidth(200);

	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	//main_vbox->setContentsMargins(0, 0, 0, 0);
	this->setLayout(main_vbox);

	QPalette pale;
	pale.setColor(QPalette::Window, Qt::transparent);
	this->chat_listWidget = new QListWidget(this);
	this->chat_listWidget->setPalette(pale);
	this->chat_listWidget->setFrameShape(QFrame::NoFrame);

	FriendChatDelegate* delegate = new  FriendChatDelegate(this);
	this->chat_listWidget->setItemDelegate(delegate);

	main_vbox->addWidget(this->chat_listWidget);

	this->increaseFriendItem(R"("D:\Doga\WallPaper\b_7ee3bc45e31b578427bd985ae04f0ab9.png")", GLOB_UserName);
	this->increaseFriendItem(R"("D:\Doga\WallPaper\b_7ee3bc45e31b578427bd985ae04f0ab9.png")", GLOB_UserName);
	this->increaseFriendItem(R"("D:\Doga\WallPaper\b_7ee3bc45e31b578427bd985ae04f0ab9.png")", GLOB_UserName);
	this->increaseFriendItem(R"("D:\Doga\WallPaper\b_7ee3bc45e31b578427bd985ae04f0ab9.png")", GLOB_UserName);
}

FriendChatList::~FriendChatList()
{
}

void FriendChatList::increaseFriendItem(const QString& userHead, const QString& username)
{
	QPixmap userHeadPixmap = RoundImage::RoundImageFromStringPath("D:/Doga/WallPaper/b_7ee3bc45e31b578427bd985ae04f0ab9.png");
	FriendChatData chat_data;
	chat_data.userHead = userHeadPixmap;
	chat_data.userMessage = "这是一条消息";
	chat_data.userName = username;
	QListWidgetItem* item = new  QListWidgetItem(this->chat_listWidget);
	item->setData(Qt::UserRole, QVariant::fromValue(chat_data));
	this->chat_listWidget->addItem(item);
}

void FriendChatList::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(220, 220, 220, 55));
	painter.drawRoundedRect(this->rect(), 15, 15);
}