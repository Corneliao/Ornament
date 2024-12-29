#include "../include/OnlineStatus.h"

OnlineStatus::OnlineStatus(QWidget* parent)
	: QWidget(parent)
{
	this->setFixedSize(80, 35);

	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);

	this->ico = new QLabel(this);
	this->ico->setFixedSize(8, 8);
	this->ico->setScaledContents(true);
	QPixmap pixmap(":/Resource/ico/TwemojiRedCircle.png");
	this->ico->setPixmap(pixmap);

	QFont font;
	font.setPixelSize(11);
	QPalette pale;
	pale.setColor(QPalette::WindowText, Qt::red);
	this->text = new QLabel("离线", this);
	this->text->setPalette(pale);
	this->text->setFont(font);
	this->text->adjustSize();

	main_lay->addSpacing(3);
	main_lay->addWidget(this->ico);
	main_lay->addSpacing(5);
	main_lay->addWidget(this->text);

	this->drawPenRect = this->rect();
	this->drawPenRect.adjust(2, 2, -2, -2);
}

OnlineStatus::~OnlineStatus()
{
}

void OnlineStatus::setOnlineStatus(bool isOnline)
{
	QPixmap pixmap;
	QPalette pale;
	this->isOnline = isOnline;
	if (this->isOnline) {
		pixmap.load(":/Resource/ico/TwemojiGreenCircle.png");
		this->ico->setPixmap(pixmap);
		pale.setColor(QPalette::WindowText, Qt::darkGreen);
		this->text->setText("在线");
	}
	else {
		pixmap.load(":/Resource/ico/TwemojiRedCircle.png");
		this->ico->setPixmap(pixmap);
		pale.setColor(QPalette::WindowText, Qt::red);
		this->text->setText("离线");
	}

	this->text->setPalette(pale);
	this->update();
}

void OnlineStatus::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	if (this->isOnline) {
		painter.setPen(QPen(Qt::green, 1));
		painter.setBrush(QColor(0, 255, 0, 7));
	}
	else {
		painter.setPen(QPen(Qt::red, 1));
		painter.setBrush(QColor(255, 0, 0, 7));
	}
	painter.drawRoundedRect(this->drawPenRect, 15, 15);
}
