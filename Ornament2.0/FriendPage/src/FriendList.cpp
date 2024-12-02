#include "../include/FriendList.h"
#include <QSystemTrayIcon>
FriendList::FriendList(QWidget* parent)
	: QWidget(parent)
{
	this->setFixedWidth(200);

	this->setWindowIcon(QIcon(":/Resource/ico/TablerBrandUnity.png"));

	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	this->setLayout(main_vbox);

	QFont font;
	font.setPixelSize(15);
	font.setBold(true);
	QPalette pale;
	pale.setColor(QPalette::WindowText, Qt::black);

	this->title = new QLabel("Friends", this);
	this->title->setFont(font);
	this->title->setPalette(pale);

	pale.setColor(QPalette::Window, Qt::transparent);
	this->friend_list = new QListWidget(this);
	this->friend_list->setPalette(pale);
	this->friend_list->setFrameShape(QFrame::NoFrame);
	FriendListDelegate* delegate = new FriendListDelegate(this);
	this->friend_list->setItemDelegate(delegate);

	main_vbox->addWidget(this->title);
	main_vbox->addSpacing(8);
	main_vbox->addWidget(friend_list);

	connect(this->friend_list, &QListWidget::itemDoubleClicked, this, &FriendList::dealItemClicked, Qt::DirectConnection);
	connect(this->friend_list, &QListWidget::itemChanged, this, &FriendList::dealItemChanged, Qt::DirectConnection);
}

FriendList::~FriendList()
{
}

void FriendList::initializeFriends(const QList<UserData> datas)
{
	for (const UserData data : datas)
		this->increaseUserFriendItem(data);
}

void FriendList::increaseUserFriendItem(const UserData& user_data)
{
	QListWidgetItem* item = new QListWidgetItem(this->friend_list);
	item->setData(Qt::UserRole, QVariant::fromValue(user_data));
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
			data.status = true;
			item->setData(Qt::UserRole, QVariant::fromValue(data));
			SystemTrayIconNotification::getInstence()->showMessage(data.userName, "已上线", data.userHead.scaled(QSize(100 * GLOB_ScaleDpi, 100 * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
			return;
		}
	}
}

UserData FriendList::getUserData(const QString& userAccount) const
{
	UserData user_data;
	for (int i = 0; i < this->friend_list->count(); i++) {
		user_data = this->friend_list->item(i)->data(Qt::UserRole).value<UserData>();
		if (user_data.userAccount == userAccount)
		{
			return user_data;
		}
	}
	return user_data;
}

void FriendList::dealItemClicked(QListWidgetItem* item)
{
	UserData  user_data = item->data(Qt::UserRole).value<UserData>();
	emit this->createChatWindowSignal(user_data);
}

void FriendList::dealItemChanged(QListWidgetItem* item)
{
	UserData  user_data = item->data(Qt::UserRole).value<UserData>();
	emit this->ItemChanged(user_data);
}

void FriendList::setUserDataForDisconnected(const QString& userAccount)
{
	UserData temp;
	for (int i = 0; i < this->friend_list->count(); i++) {
		temp = this->friend_list->item(i)->data(Qt::UserRole).value<UserData>();
		if (temp.userAccount == userAccount) {
			temp.status = false;
			QPixmap pixmap(":/Resource/ico/TwemojiRedCircle.png");
			temp.status_ico = pixmap;
			temp.status_text = "离线";
			this->friend_list->item(i)->setData(Qt::UserRole, QVariant::fromValue(temp));
			return;
		}
	}
}

void FriendList::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	//painter.setBrush(QColor(220, 220, 220, 55));
	painter.setBrush(Qt::transparent);
	painter.drawRoundedRect(this->rect(), 15, 15);
}