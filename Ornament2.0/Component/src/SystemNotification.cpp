#include "../include/SystemNotification.h"

SystemNotification::SystemNotification(QWidget* parent)
	: QWidget(parent)
{
	this->setFixedSize(200, parent->height() - 30);
	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	main_vbox->setContentsMargins(9, 0, 9, 9);
	this->setLayout(main_vbox);

	this->title_bar = new NormalTitleBar("通知", this);
	this->title_bar->setCloseVisible(true);

	QPalette pale;

	QFont font;
	font.setPixelSize(11);
	this->userApplicationNotification = new QLabel("好友通知", this);
	this->userApplicationNotification->setFont(font);
	this->userApplicationNotification->adjustSize();
	pale.setColor(QPalette::Window, Qt::transparent);
	this->userApplicationNotificationItemList = new QListWidget(this);
	this->userApplicationNotificationItemList->setPalette(pale);
	this->userApplicationNotificationItemList->setFrameShape(QFrame::NoFrame);

	this->system_Notification = new QLabel("系统通知", this);
	this->system_Notification->setFont(font);
	this->system_Notification->adjustSize();

	this->systemNotificationItemList = new QListWidget(this);
	this->systemNotificationItemList->setFrameShape(QFrame::NoFrame);
	this->systemNotificationItemList->setPalette(pale);

	main_vbox->addWidget(this->title_bar);
	main_vbox->addSpacing(5);
	main_vbox->addWidget(this->userApplicationNotification, 0, Qt::AlignLeft);
	main_vbox->addWidget(this->userApplicationNotificationItemList);
	main_vbox->addWidget(this->system_Notification);
	main_vbox->addWidget(this->systemNotificationItemList);

	this->shadow = new QGraphicsDropShadowEffect(this);
	this->shadow->setOffset(0, 0);
	this->shadow->setBlurRadius(8);
	this->shadow->setColor(Qt::gray);
	this->setGraphicsEffect(this->shadow);

}

SystemNotification::~SystemNotification()
{
}

void SystemNotification::IncreaseUserApplicationItem(const UserData& user_data)
{
	QListWidgetItem* item = new QListWidgetItem(this->userApplicationNotificationItemList);
	item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
	item->setData(Qt::UserRole, QVariant::fromValue(user_data));
	UserApplicationItemWidget* itemWidget = new UserApplicationItemWidget(user_data, this);
	item->setSizeHint(QSize(itemWidget->size()));
	this->userApplicationNotificationItemList->addItem(item);
	this->userApplicationNotificationItemList->setItemWidget(item, itemWidget);
	connect(itemWidget, &UserApplicationItemWidget::agreenApplicationSignal, this, &SystemNotification::dealAgreeApplication, Qt::DirectConnection);
	connect(itemWidget, &UserApplicationItemWidget::disAgreeApplicationSignal, this, &SystemNotification::removeApplicationItem, Qt::DirectConnection);
	connect(itemWidget, &UserApplicationItemWidget::updateCurrentUserFriendList, this, &SystemNotification::updateFriendList, Qt::DirectConnection);
}

void SystemNotification::dealAgreeApplication(const QString& cronyAccount)
{
	emit this->agreeApplicationSignal(cronyAccount);
	this->removeApplicationItem();
}

void SystemNotification::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::white);
	painter.drawRoundedRect(this->rect(), 10, 10);
}

void SystemNotification::removeApplicationItem()
{
	UserApplicationItemWidget* itemWidget = qobject_cast<UserApplicationItemWidget*>(this->sender());
	for (int i = 0; i < this->userApplicationNotificationItemList->count(); i++) {
		QListWidgetItem* item = this->userApplicationNotificationItemList->item(i);
		if (itemWidget == this->userApplicationNotificationItemList->itemWidget(item)) {
			itemWidget->deleteLater();
			itemWidget = Q_NULLPTR;
		}

		delete item;
		item = Q_NULLPTR;
	}
	if (this->userApplicationNotificationItemList->count() <= 0)
		emit this->NotificationEmptySignal();
}
