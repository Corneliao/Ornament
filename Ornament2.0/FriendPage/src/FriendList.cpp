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
	this->friend_listView = new QListView(this);
	this->friend_listView->setPalette(pale);
	this->friend_listView->setFrameShape(QFrame::NoFrame);
	FriendListDelegate* delegate = new FriendListDelegate(this);
	this->friend_listView->setItemDelegate(delegate);
	this->model = new QStandardItemModel(this);
	this->friend_listView->setModel(this->model);
	main_vbox->addWidget(friend_listView);

	//this->trayIcon = new QSystemTrayIcon(this);
	//this->trayIcon->setIcon(QIcon(":/Resource/ico/TablerBrandUnity.png"));
	//this->trayIcon->setToolTip("Ornament");
	//this->trayIcon->show();
}

FriendList::~FriendList()
{
}

void FriendList::initializeFriends(const QList<FriendListData> datas)
{
	for (const FriendListData data : datas)
		this->increaseUserFriendItem(data);
}

void FriendList::increaseUserFriendItem(const FriendListData friend_data)
{
	QStandardItem* item = new QStandardItem();
	item->setData(QVariant::fromValue(friend_data), Qt::UserRole);
	this->model->appendRow(item);
}

void FriendList::updateFriendCurrentStatus(const QString& cronyAccount)
{
	if (this->model->rowCount() == 0)
		return;

	for (int i = 0; i < this->model->rowCount(); i++) {
		QStandardItem* item = this->model->item(i, 0);
		FriendListData data = item->data(Qt::UserRole).value<FriendListData>();
		if (data.userAccount == cronyAccount) {
			QPixmap pixmap(":/Resource/ico/TwemojiGreenCircle.png");
			data.status_ico = pixmap;
			data.status_text = "在线";
			item->setData(QVariant::fromValue(data), Qt::UserRole);
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