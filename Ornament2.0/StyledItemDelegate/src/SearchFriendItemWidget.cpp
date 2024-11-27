#include "../include/SearchFriendItemWidget.h"

SearchFriendItemWidget::SearchFriendItemWidget(const SearchFriendData& friend_data, QWidget* parent)
	: QWidget(parent)
{
	this->m_friend_data = friend_data;
	this->setFixedHeight(50);
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);

	//绘制头像
	this->userHead = new QLabel(this);
	this->userHead->setFixedSize(30, 30);
	this->userHead->setScaledContents(true);
	QPixmap pixmap = RoundImage::RoundImageFromByteArray(friend_data.imagebytes);
	this->userHead->setPixmap(pixmap);

	//
	this->userName = new QLabel(friend_data.userName, this);
	this->userName->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	this->userName->setFixedWidth(200);

	this->statusIco = new QLabel(this);
	this->statusIco->setFixedSize(8, 8);
	this->statusIco->setScaledContents(true);

	this->statusText = new  QLabel(this);

	if (friend_data.status) {
		pixmap.load(":/Resource/ico/TwemojiGreenCircle.png");
		this->statusIco->setPixmap(pixmap);
		this->statusText->setText("在线");
	}
	else {
		pixmap.load(":/Resource/ico/TwemojiRedCircle.png");
		this->statusIco->setPixmap(pixmap);
		this->statusText->setText("离线 ");
	}

	this->button = new  SearchAddFriendButton(this);
	this->button->installEventFilter(this);

	main_lay->addWidget(this->userHead);
	main_lay->addSpacing(5);
	main_lay->addWidget(this->userName);
	main_lay->addStretch();
	main_lay->addWidget(this->statusIco);
	main_lay->addSpacing(5);
	main_lay->addWidget(this->statusText);
	main_lay->addSpacing(20);
	main_lay->addWidget(this->button);
}

SearchFriendItemWidget::~SearchFriendItemWidget()
{
}

void SearchFriendItemWidget::ExistTheUser()
{
	this->button->hide();
}

bool SearchFriendItemWidget::eventFilter(QObject* target, QEvent* event)
{
	if (target == this->button) {
		if (event->type() == QEvent::MouseButtonPress) {
			emit this->SendFreindApplication(this->m_friend_data.userAccount);
			return true;
		}
	}
	return QWidget::eventFilter(target, event);
}

SearchAddFriendButton::SearchAddFriendButton(QWidget* parent) :QWidget(parent)
{
	this->setFixedWidth(60);
	this->setCursor(Qt::PointingHandCursor);
	QPalette pale;
	pale.setColor(QPalette::Text, Qt::white);
	QFont font;
	font.setBold(true);
	font.setPixelSize(12);
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);

	this->text = new QLabel("添加", this);
	this->text->setPalette(pale);
	this->text->setFont(font);
	main_lay->addWidget(this->text, 0, Qt::AlignCenter);
}

void SearchAddFriendButton::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor::fromString("#87CEFA"));
	painter.drawRoundedRect(this->rect(), 10, 10);
}
