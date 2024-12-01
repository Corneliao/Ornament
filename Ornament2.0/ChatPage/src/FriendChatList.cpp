#include "../include/FriendChatList.h"

FriendChatList::FriendChatList(QWidget* parent)
	: QWidget(parent)
{
	this->setFixedWidth(200);

	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	this->setLayout(main_vbox);

	QFont font;
	font.setBold(true);
	font.setPixelSize(14);
	QPalette pale;
	pale.setColor(QPalette::WindowText, Qt::black);

	QHBoxLayout* lay = new QHBoxLayout;

	this->title = new QLabel("Message", this);
	this->title->setFont(font);
	this->title->setPalette(pale);

	this->edit = new QLabel(this);
	this->edit->setFixedSize(15, 15);
	this->edit->setScaledContents(true);
	QPixmap pixmap(":/Resource/ico/friend_edit_sel.png");
	this->edit->setPixmap(pixmap);

	lay->addWidget(this->title);
	lay->addStretch();
	lay->addWidget(this->edit);

	pale.setColor(QPalette::Window, Qt::transparent);
	this->chat_listWidget = new QListWidget(this);
	this->chat_listWidget->setPalette(pale);
	this->chat_listWidget->setFrameShape(QFrame::NoFrame);

	FriendChatDelegate* delegate = new  FriendChatDelegate(this);
	this->chat_listWidget->setItemDelegate(delegate);

	main_vbox->addLayout(lay);
	main_vbox->addSpacing(8);
	main_vbox->addWidget(this->chat_listWidget);

	connect(this->chat_listWidget, &QListWidget::itemClicked, this, &FriendChatList::dealItemClicked, Qt::DirectConnection);
}

FriendChatList::~FriendChatList()
{
}

void FriendChatList::increaseFriendItem(QListWidgetItem* item)
{
	//判断聊天窗口是否已存在
	//QListWidgetItem* item = new  QListWidgetItem(this->chat_listWidget);
	//item->setData(Qt::UserRole, QVariant::fromValue(user_data));
	this->chat_listWidget->addItem(item);
}

int FriendChatList::isExistFriendChatItem(const QString& account)
{
	for (int i = 0; i < this->chat_listWidget->count(); i++) {
		QListWidgetItem* item = this->chat_listWidget->item(i);
		UserData user_data = item->data(Qt::UserRole).value<UserData>();
		if (user_data.userAccount == account) {
			return i;
		}
	}
	return -1;
}

void FriendChatList::setItemSelected(const QString& account)
{
	for (int i = 0; i < this->chat_listWidget->count(); i++) {
		QListWidgetItem* item = this->chat_listWidget->item(i);
		UserData user_data = item->data(Qt::UserRole).value<UserData>();
		if (user_data.userAccount == account) {
			item->setSelected(true);
			return;
		}
	}
	;
}

void FriendChatList::setItemData(int index, const UserData& user_data)
{
	UserData item_data = this->chat_listWidget->item(index)->data(Qt::UserRole).value<UserData>();
	item_data.alignment = Qt::AlignLeading;
	item_data.userMessage = user_data.userMessage;
	this->chat_listWidget->item(index)->setData(Qt::UserRole, QVariant::fromValue(item_data));
}

void FriendChatList::dealItemClicked(const QListWidgetItem* item)
{
	UserData user_data = item->data(Qt::UserRole).value<UserData>();
	emit this->FriendChatItemChanged(user_data);
}

void FriendChatList::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.save();
	painter.setPen(QColor(0, 0, 0, 20));
	painter.setBrush(Qt::NoBrush);
	painter.drawLine(QPoint(this->rect().topLeft()), QPoint(this->rect().bottomLeft()));
	painter.restore();

	painter.save();
	painter.setPen(Qt::NoPen);
	//painter.setBrush(QColor(220, 220, 220, 55));
	painter.setBrush(Qt::transparent);
	painter.drawRoundedRect(this->rect(), 15, 15);
	painter.restore();
}
