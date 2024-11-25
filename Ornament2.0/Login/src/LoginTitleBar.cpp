#include "../include/LoginTitleBar.h"

LoginTitleBar::LoginTitleBar(QWidget* parent)
	: QWidget(parent)
{
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);

	this->close_window = new QLabel(this);
	this->close_window->setCursor(Qt::PointingHandCursor);
	this->close_window->setFixedSize(25, 25);
	this->close_window->setScaledContents(true);
	QPixmap pixmap(":/Resource/ico/IconamoonClose.png");
	pixmap = pixmap.scaled(QSize(this->close_window->size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	this->close_window->setPixmap(pixmap);

	main_lay->addStretch();
	main_lay->addWidget(this->close_window);

	//安装过滤器
	this->close_window->installEventFilter(this);
}

LoginTitleBar::~LoginTitleBar()
{
}

bool LoginTitleBar::eventFilter(QObject* target, QEvent* event)
{
	if (target == this->close_window) {
		if (event->type() == QEvent::MouseButtonPress) {
			emit this->closeWindowSignal();
			return true;
		}
	}
	return QWidget::eventFilter(target, event);
}