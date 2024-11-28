#include "../include/AddFriend.h"

AddFriend::AddFriend(QDialog* parent)
	: FranelessWindowForDialog(parent)
{
	this->setFocusPolicy(Qt::ClickFocus);
	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	this->setLayout(main_vbox);
	this->search = new LineEdit(this);

	this->listWidget = new QListWidget(this);
	this->listWidget->setFrameShape(QFrame::NoFrame);
	main_vbox->addWidget(this->search, 0, Qt::AlignTop);
	main_vbox->addSpacing(5);
	main_vbox->addWidget(this->listWidget);

	this->notification = new Notification(this);
	this->notification->hide();

	this->noticeAnimation = new QPropertyAnimation(this->notification, "geometry", this);
	this->noticeAnimation->setEasingCurve(QEasingCurve::InOutSine);
	this->noticeAnimation->setDuration(500);

	this->noticeBackTimer = new QTimer(this);
	this->noticeBackTimer->setInterval(1000);
	this->noticeBackTimer->setSingleShot(true);
	connect(this->noticeBackTimer, &QTimer::timeout, this, &AddFriend::noticeAnimationTimerOut, Qt::DirectConnection);

	this->windowSizeAnimation = new QTimeLine(500, this);
	this->windowSizeAnimation->setUpdateInterval(0);
	this->windowSizeAnimation->setEasingCurve(QEasingCurve::InOutSine);
	this->windowSizeAnimation->setFrameRange(60, 300);

	connect(this->windowSizeAnimation, &QTimeLine::frameChanged, this, &AddFriend::SizeAnimationFrameChanged, Qt::DirectConnection);
	connect(this->search, &LineEdit::searchFriend, this, &AddFriend::searchFriend, Qt::DirectConnection);
	connect(this->search, &LineEdit::textEmpty, this, &AddFriend::SearchBarTextEmpty, Qt::DirectConnection);
	connect(this->search, &LineEdit::hideAddFriend, this, &AddFriend::deleteLater, Qt::DirectConnection);
}

AddFriend::~AddFriend()
{
}

void AddFriend::increaseSearchMember(const SearchFriendData& data)
{
	if (data.userAccount.isEmpty())
		return;
	if (this->listWidget->count() != 0) {
		for (int i = 0; i < this->listWidget->count(); i++) {
			QListWidgetItem* item = this->listWidget->item(i);
			if (item) {
				SearchFriendItemWidget* itemWidget = qobject_cast<SearchFriendItemWidget*>(this->listWidget->itemWidget(item));
				itemWidget->disconnect(this);
				itemWidget->deleteLater();
				itemWidget = Q_NULLPTR;
			}
			delete item;
			item = Q_NULLPTR;
		}
	}

	QListWidgetItem* item = new QListWidgetItem(this->listWidget);
	item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
	item->setData(Qt::UserRole, QVariant::fromValue(data));
	SearchFriendItemWidget* itemWidget = new SearchFriendItemWidget(data, this);
	item->setSizeHint(itemWidget->size());
	this->listWidget->addItem(item);
	this->listWidget->setItemWidget(item, itemWidget);
	connect(itemWidget, &SearchFriendItemWidget::SendFreindApplication, this, &AddFriend::sendFriendApplication, Qt::DirectConnection);
}

void AddFriend::sendApplicationSucceedAnimation(bool isSucceed)
{
	if (isSucceed)
		this->notification->setText("发送好友申请成功");

	else
		this->notification->setText("请勿重复发送好友申请");
	this->noticeAnimation->setDirection(QPropertyAnimation::Forward);
	this->noticeAnimation->setStartValue(QRect(QPoint(this->rect().center().x() - (this->notification->width() / 2), this->rect().top() - (this->notification->height())), QSize(this->notification->size())));
	this->noticeAnimation->setEndValue(QRect(QPoint(this->rect().center().x() - (this->notification->width() / 2), this->rect().top() + 20), QSize(this->notification->size())));
	this->noticeAnimation->stop();
	this->noticeAnimation->start();
	this->notification->show();
	this->noticeBackTimer->stop();
	this->noticeBackTimer->start();
}

void AddFriend::ExistTheUser()
{
	this->notification->setText("你已存在此好友，无需再次添加");
	this->noticeAnimation->setDirection(QPropertyAnimation::Forward);
	this->noticeAnimation->setStartValue(QRect(QPoint(this->rect().center().x() - (this->notification->width() / 2), this->rect().top() - (this->notification->height())), QSize(this->notification->size())));
	this->noticeAnimation->setEndValue(QRect(QPoint(this->rect().center().x() - (this->notification->width() / 2), this->rect().top() + 20), QSize(this->notification->size())));
	this->noticeAnimation->stop();
	this->noticeAnimation->start();
	this->notification->show();
	this->noticeBackTimer->stop();
	this->noticeBackTimer->start();
}

void AddFriend::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::white);
	painter.drawRoundedRect(this->rect(), 15, 15);
}

void AddFriend::showEvent(QShowEvent*)
{
	this->setFixedSize(400, 60);
	this->move((screenSize.width()-this->width())/2, (screenSize.height() - this->height()) / 2);
}

void AddFriend::searchFriend(const QString& userAccount)
{
	this->windowSizeAnimation->setDirection(QTimeLine::Forward);
	this->windowSizeAnimation->stop();
	this->windowSizeAnimation->start();

	if (GLOB_IsConnectedServer)
		emit this->searchFriendSignal(userAccount);
}

void AddFriend::SizeAnimationFrameChanged(int frame)
{
	this->setFixedSize(this->width(), frame);
}

void AddFriend::noticeAnimationTimerOut()
{
	this->noticeAnimation->setDirection(QPropertyAnimation::Backward);
	this->noticeAnimation->stop();
	this->noticeAnimation->start();
}

void AddFriend::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		this->isDraging = true;
		this->startPoint = event->globalPosition().toPoint();
		m_offset = this->startPoint - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void AddFriend::mouseReleaseEvent(QMouseEvent* event)
{
	this->isDraging = false;
	QWidget::mouseReleaseEvent(event);
}

void AddFriend::mouseMoveEvent(QMouseEvent* event)
{
	if (this->isDraging && this->underMouse()) {
		move(event->globalPos() - m_offset);
		event->accept();
	}

	QWidget::mouseMoveEvent(event);
}

void AddFriend::SearchBarTextEmpty()
{
	if (this->height() <= 60)
		return;
	this->windowSizeAnimation->setDirection(QTimeLine::Backward);
	this->windowSizeAnimation->stop();
	this->windowSizeAnimation->start();

	if (this->listWidget->count() != 0) {
		for (int i = 0; i < this->listWidget->count(); i++) {
			QListWidgetItem* item = this->listWidget->item(i);
			if (item) {
				SearchFriendItemWidget* itemWidget = qobject_cast<SearchFriendItemWidget*>(this->listWidget->itemWidget(item));
				itemWidget->deleteLater();
				itemWidget = Q_NULLPTR;
			}
			delete item;
			item = Q_NULLPTR;
		}
	}
}
