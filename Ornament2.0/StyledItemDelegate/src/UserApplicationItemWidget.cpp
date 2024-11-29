#include "../include/UserApplicationItemWidget.h"

UserApplicationItemWidget::UserApplicationItemWidget(const UserData& user_data, QWidget* parent)
	: QWidget(parent)
{
	//this->setFixedHeight(80);

	this->m_userData = user_data;

	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	this->setLayout(main_vbox);

	this->userHead = new QLabel(this);
	this->userHead->setFixedSize(40, 40);
	this->userHead->setScaledContents(true);
	this->userHead->setPixmap(user_data.userHead.scaled(QSize(this->userHead->size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

	this->userName = new QLabel(user_data.userName, this);
	this->userName->adjustSize();

	QHBoxLayout* lay = new QHBoxLayout;
	lay->addWidget(this->userHead);

	QVBoxLayout* vbox = new QVBoxLayout;

	QFont font;
	font.setPixelSize(10);
	this->application = new QLabel("向你发来好友申请", this);
	this->application->setFont(font);
	this->application->adjustSize();

	vbox->addWidget(this->userName);
	vbox->addWidget(this->application);
	vbox->addSpacing(5);
	lay->addLayout(vbox);

	QHBoxLayout* lay_2 = new QHBoxLayout;
	this->agree = new UserApplicationButton("同意", this);
	this->agree->installEventFilter(this);
	this->disAgree = new UserApplicationButton("拒绝", this);
	this->disAgree->installEventFilter(this);
	this->disAgree->setBackgroundColor(Qt::red);

	lay_2->addWidget(this->agree);
	lay_2->addWidget(this->disAgree);

	main_vbox->addLayout(lay);
	main_vbox->addLayout(lay_2);

	this->adjustSize();
}

UserApplicationItemWidget::~UserApplicationItemWidget()
{
}

bool UserApplicationItemWidget::eventFilter(QObject* target, QEvent* event)
{
	if (this->agree == target) {
		if (event->type() == QEvent::MouseButtonPress) {
			emit this->agreenApplicationSignal(this->m_userData.userAccount);
			return true;
		}
	}
	if (target == this->disAgree) {
		if (event->type() == QEvent::MouseButtonPress) {
			emit this->disAgreeApplicationSignal();
			return true;
		}
	}
	return QWidget::eventFilter(target, event);
}

UserApplicationButton::UserApplicationButton(const QString& button_text, QWidget* parent)
{
	this->setCursor(Qt::PointingHandCursor);
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);

	QFont font;
	font.setBold(true);
	font.setPixelSize(10);
	QPalette pale;
	pale.setColor(QPalette::Text, Qt::white);
	this->buttonText = new QLabel(button_text, this);
	this->buttonText->setFont(font);
	this->buttonText->setPalette(pale);
	this->buttonText->adjustSize();

	main_lay->addWidget(this->buttonText, 0, Qt::AlignCenter);
}

UserApplicationButton::~UserApplicationButton()
{
}

void UserApplicationButton::setBackgroundColor(const QColor& color)
{
	this->backgroundColor = color;
	this->update();
}

void UserApplicationButton::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(this->backgroundColor);
	painter.drawRoundedRect(this->rect(), 10, 10);
}
