#include "../include/NormalTitleBar.h"

NormalTitleBar::NormalTitleBar(const QString& text, QWidget* parent)
	: QWidget(parent)
{
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	//main_lay->setContentsMargins(9, 0, 9, 9);
	this->setLayout(main_lay);

	QFont font;
	font.setPixelSize(12);
	font.setBold(true);
	this->title = new QLabel(text, this);
	this->title->setFont(font);
	this->title->adjustSize();

	this->close_window = new QLabel(this);
	this->close_window->setCursor(Qt::PointingHandCursor);
	this->close_window->setFixedSize(25, 25);
	this->close_window->setScaledContents(true);
	QPixmap pixmap(":/Resource/ico/IconamoonClose.png");
	pixmap = pixmap.scaled(QSize(this->close_window->size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	this->close_window->setPixmap(pixmap);

	main_lay->addWidget(this->title);
	main_lay->addStretch();
	main_lay->addWidget(this->close_window);

	//安装过滤器
	this->close_window->installEventFilter(this);
}

NormalTitleBar::~NormalTitleBar()
{
}

void NormalTitleBar::setCloseVisible(bool visible)
{
	if (visible)
		this->close_window->hide();
	else
		this->close_window->show();
}

bool NormalTitleBar::eventFilter(QObject* target, QEvent* event)
{
	if (target == this->close_window) {
		if (event->type() == QEvent::MouseButtonPress) {
			emit this->closeWindowSignal();
			return true;
		}
	}
	return QWidget::eventFilter(target, event);
}
