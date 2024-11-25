#include "../include/DefaultWindow.h"

DefaultWindow::DefaultWindow(QWidget *parent)
	: QWidget(parent)
{
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);
	
	this->NoChatWindowIco = new QLabel(this);
	this->NoChatWindowIco->setFixedSize(80, 80);
	this->NoChatWindowIco->setScaledContents(true);
	QPixmap ico_pixmap(":/Resource/ico/TablerBrandUnity_empty.png");
	ico_pixmap = ico_pixmap.scaled(QSize(this->NoChatWindowIco->size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	this->NoChatWindowIco->setPixmap(ico_pixmap);

	main_lay->addWidget(this->NoChatWindowIco, 0, Qt::AlignCenter);
}

DefaultWindow::~DefaultWindow()
{
}
