#include "../include/FriendList.h"
#include <QSystemTrayIcon>
FriendList::FriendList(QWidget* parent)
	: QWidget(parent)
{
	this->setFixedWidth(200);

	this->setWindowIcon(QIcon(":/Resource/ico/TablerBrandUnity.png"));

	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	this->setLayout(main_vbox);

	QPalette pale;
	pale.setColor(QPalette::Window, Qt::transparent);
	this->friend_list = new QListWidget(this);
	this->friend_list->setPalette(pale);
	this->friend_list->setFrameShape(QFrame::NoFrame);
	FriendListDelegate* delegate = new FriendListDelegate(this);
	this->friend_list->setItemDelegate(delegate);
	main_vbox->addWidget(friend_list);

	connect(this->friend_list, &QListWidget::itemDoubleClicked, this, &FriendList::createChatWindowSignal, Qt::DirectConnection);
}

FriendList::~FriendList()
{
}

void FriendList::initializeFriends(const QList<UserData> datas)
{
	for (const UserData data : datas)
		this->increaseUserFriendItem(data);
}

void FriendList::increaseUserFriendItem(const UserData friend_data)
{
	QListWidgetItem* item = new QListWidgetItem(this->friend_list);
	item->setData(Qt::UserRole, QVariant::fromValue(friend_data));
	this->friend_list->addItem(item);
}

void FriendList::updateFriendCurrentStatus(const QString& cronyAccount)
{
	if (this->friend_list->count() == 0)
		return;

	for (int i = 0; i < this->friend_list->count(); i++) {
		QListWidgetItem* item = this->friend_list->item(i);
		UserData data = item->data(Qt::UserRole).value<UserData>();
		if (data.userAccount == cronyAccount) {
			QPixmap pixmap(":/Resource/ico/TwemojiGreenCircle.png");
			data.status_ico = pixmap;
			data.status_text = "在线";
			item->setData(Qt::UserRole, QVariant::fromValue(data));
			//this->trayIcon->showMessage(data.userName, "你的好友已上线", data.userHead.scaled(QSize(100 * GLOB_ScaleDpi, 100 * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
			SystemTrayIconNotification::getInstence()->showMessage(data.userName, "你的好友已上线", data.userHead.scaled(QSize(100 * GLOB_ScaleDpi, 100 * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
			return;
		}
	}
}

void FriendList::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(220, 220, 220, 55));
	painter.drawRoundedRect(this->rect(), 15, 15);
}
