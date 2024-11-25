#include "../include/TitleTool.h"

TitleTool::TitleTool(QWidget* parent)
	: QWidget(parent)
{
	this->setFixedSize(120, 150);

	this->setMouseTracking(true);
	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	main_vbox->setContentsMargins(3, 7, 3, 3);
	this->setLayout(main_vbox);

	this->tool = new Tool(this);

	main_vbox->addWidget(this->tool);
	main_vbox->addStretch();

	this->shadow = new QGraphicsDropShadowEffect(this);
	this->shadow->setBlurRadius(15);
	this->shadow->setOffset(0, 0);
	this->shadow->setColor(Qt::lightGray);
	this->setGraphicsEffect(this->shadow);

	connect(this->tool, &Tool::minWindowSignal, this, &TitleTool::minWindowSignal, Qt::DirectConnection);
	connect(this->tool, &Tool::maxWindowSignal, this, &TitleTool::maxWindowSignal, Qt::DirectConnection);
	connect(this->tool, &Tool::closeWindowSignal, this, &TitleTool::closeWindowSignal, Qt::DirectConnection);
}

TitleTool::~TitleTool()
{
}

void TitleTool::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::white);
	painter.drawRoundedRect(this->rect(), 10, 10);
}

Tool::Tool(QWidget* parent)
{
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	main_lay->setContentsMargins(0, 0, 0, 0);
	this->setLayout(main_lay);

	this->minWindow = new QLabel(this);
	this->minWindow->setFixedSize(18, 18);
	this->minWindow->setScaledContents(true);
	QPixmap pixmap(":/Resource/ico/MingcuteMinimizeLine.png");
	this->minWindow->setPixmap(pixmap);
	this->minWindow->setCursor(Qt::PointingHandCursor);
	this->minWindow->installEventFilter(this);

	this->maxWindow = new QLabel(this);
	this->maxWindow->setFixedSize(18, 18);
	this->maxWindow->setScaledContents(true);
	pixmap.load(":/Resource/ico/FluentMaximize16Regular.png");
	this->maxWindow->setPixmap(pixmap);
	this->maxWindow->setCursor(Qt::PointingHandCursor);
	this->maxWindow->installEventFilter(this);

	this->closeWindow = new QLabel(this);
	this->closeWindow->setFixedSize(18, 18);
	this->closeWindow->setScaledContents(true);
	pixmap.load(":/Resource/ico/IconamoonClose.png");
	this->closeWindow->setPixmap(pixmap);
	this->closeWindow->setCursor(Qt::PointingHandCursor);
	this->closeWindow->installEventFilter(this);

	main_lay->addWidget(this->minWindow);
	main_lay->addWidget(this->maxWindow);
	main_lay->addWidget(this->closeWindow);
}

bool Tool::eventFilter(QObject* target, QEvent* event)
{
	if (target == this->minWindow) {
		if (event->type() == QEvent::MouseButtonPress) {
			emit this->minWindowSignal();
			return true;
		}
	}
	if (target == this->maxWindow) {
		if (event->type() == QEvent::MouseButtonPress) {
			emit this->maxWindowSignal();
			return true;
		}
	}
	if (target == this->closeWindow) {
		if (event->type() == QEvent::MouseButtonPress) {
			emit this->closeWindowSignal();
			return true;
		}
	}
	return QWidget::eventFilter(target, event);
}