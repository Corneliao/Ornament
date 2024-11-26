#include "../include/ApplicationTitleBar.h"

ApplicationTitleBar::ApplicationTitleBar(QWidget* parent)
	: QWidget(parent)
{
	this->dpi = this->devicePixelRatioF();

	QHBoxLayout* main_lay = new QHBoxLayout(this);
	main_lay->setContentsMargins(25, 11, 25, 11);
	this->setLayout(main_lay);

	this->window_ico = new QLabel(this);
	this->window_ico->setFixedSize(25, 25);
	this->window_ico->setScaledContents(true);

	QPixmap pixmap(":/Resource/ico/TablerBrandUnity.png");
	pixmap = pixmap.scaled(QSize(this->window_ico->width() * dpi, this->window_ico->height() * dpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	this->window_ico->setPixmap(pixmap);

	QFont font;
	font.setBold(true);
	font.setPixelSize(15);
	QPalette pale;
	pale.setColor(QPalette::WindowText, QColor::fromString("#1e6efe"));

	this->window_text = new QLabel(this);
	this->window_text->setText("Ornament");
	this->window_text->setAlignment(Qt::AlignCenter);
	this->window_text->setPalette(pale);
	this->window_text->setFont(font);
	this->window_text->adjustSize();

	this->online_status = new OnlineStatus(this);

	this->search = new QLabel(this);
	this->search->setCursor(Qt::PointingHandCursor);
	this->search->setFixedSize(18, 18);
	this->search->setScaledContents(true);
	pixmap.load(":/Resource/ico/RiSearchLine.png");
	this->search->setPixmap(pixmap);
	this->search->installEventFilter(this);

	this->notification = new QLabel(this);
	this->notification->setFixedSize(18, 18);
	this->notification->setScaledContents(true);
	pixmap.load(":/Resource/ico/RiNotification3Line.png");
	this->notification->setPixmap(pixmap);

	//--------------------

	this->user_head = new QLabel(this);
	this->user_head->setFixedSize(40, 40);
	this->user_head->setScaledContents(true);
	this->user_head_pixmap.load(":/Resource/ico/TablerBrandUnity.png");
	this->user_head->setPixmap(this->user_head_pixmap);

	this->unfold = new QLabel(this);
	this->unfold->setCursor(Qt::PointingHandCursor);
	this->unfold->setFixedSize(18, 18);
	this->unfold->setScaledContents(true);
	pixmap.load(":/Resource/ico/IconamoonArrowDown2Bold.png");
	pixmap = this->setIconDpi(this->unfold->size(), pixmap);
	this->unfold->setPixmap(pixmap);
	this->unfold->installEventFilter(this);

	main_lay->addWidget(this->window_ico);
	main_lay->addSpacing(5);
	main_lay->addWidget(this->window_text);
	main_lay->addStretch();
	main_lay->addWidget(this->online_status);
	main_lay->addSpacing(15);
	main_lay->addWidget(this->search);
	main_lay->addSpacing(25);
	main_lay->addWidget(this->notification);
	main_lay->addSpacing(25);
	main_lay->addWidget(this->user_head);
	main_lay->addSpacing(5);
	main_lay->addWidget(this->unfold);
}

ApplicationTitleBar::~ApplicationTitleBar()
{
}

void ApplicationTitleBar::setUserHead(const QByteArray& imagebytes)
{
	this->user_head_pixmap = RoundImage::RoundImageFromByteArray(imagebytes);
	this->user_head->setPixmap(this->user_head_pixmap);
}

void ApplicationTitleBar::setUnfoldIcon(bool isShow)
{
	QPixmap  pixmap;
	if (isShow)
		pixmap.load(":/Resource/ico/IconamoonArrowUp2Bold.png");

	else
		pixmap.load(":/Resource/ico/IconamoonArrowDown2Bold.png");
	this->unfold->setPixmap(pixmap);
}

void ApplicationTitleBar::setOnlineStatus(bool isOnline)
{
	this->online_status->setOnlineStatus(isOnline);
}

QPixmap ApplicationTitleBar::setIconDpi(const QSize& size, const QPixmap& pixmap)
{
	if (pixmap.isNull())
		return QPixmap();
	QPixmap  resultPixmap = pixmap.scaled(size * GLOB_ScaleDpi, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	return  resultPixmap;
}

bool ApplicationTitleBar::eventFilter(QObject* target, QEvent* event)
{
	if (target == this->unfold) {
		if (event->type() == QEvent::MouseButtonPress) {
			emit this->showToolSignal();
			return true;
		}
	}
	if (target == this->search) {
		if (event->type() == QEvent::MouseButtonPress) {
			emit this->showAddFriendSignal();
			return true;
		}
	}
	return QWidget::eventFilter(target, event);
}

void ApplicationTitleBar::paintEvent(QPaintEvent*)
{
	/*QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(Qt::lightGray, 0.4));
	painter.setBrush(Qt::NoBrush);
	painter.drawLine(QPoint(this->rect().bottomLeft()), QPoint(this->rect().bottomRight()));*/
}