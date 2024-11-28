#include "../include/ChatWindow.h"

ChatWindow::ChatWindow(const UserData& user_data, QWidget* parent)
	: QWidget(parent)
{
	this->m_userData = user_data;
	this->currentChatWindowIndex = user_data.index;
	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	//main_vbox->setContentsMargins(0, 0, 0, 0);
	this->setLayout(main_vbox);

	this->chat_title = new ChatTitle(user_data.userName, user_data.userHead, this);
	main_vbox->addWidget(this->chat_title);
	main_vbox->addStretch();
}

ChatWindow::~ChatWindow()
{
}

UserData ChatWindow::currentUserData() const
{
	return m_userData;
}

ChatTitle::ChatTitle(const QString& user_name, const QPixmap& user_head, QWidget* parent) :QWidget(parent)
{
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);

	this->userHead = new QLabel(this);
	this->userHead->setFixedSize(30, 30);
	this->userHead->setScaledContents(true);
	this->userHead->setPixmap(user_head.scaled(QSize(this->userHead->size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

	this->userName = new QLabel(user_name, this);
	this->userName->adjustSize();
	main_lay->addWidget(this->userHead);
	main_lay->addWidget(this->userName);
	main_lay->addStretch();

	this->shadow = new QGraphicsDropShadowEffect(this);
	this->shadow->setOffset(0, 0);
	this->shadow->setColor(Qt::gray);
	this->shadow->setBlurRadius(8);
	this->setGraphicsEffect(this->shadow);
}

ChatTitle::~ChatTitle()
{
}

void ChatTitle::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::white);
	painter.drawRoundedRect(this->rect(), 10, 10);
}