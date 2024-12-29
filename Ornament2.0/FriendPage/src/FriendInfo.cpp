#include "../include/FriendInfo.h"

FriendInfo::FriendInfo(QWidget* parent)
	: QWidget(parent)
{
	QVBoxLayout* vbox = new QVBoxLayout(this);
	this->setLayout(vbox);

	QPalette pale;
	pale.setColor(QPalette::Window, Qt::red);
	this->friendHeader = new QLabel(this);
	this->friendHeader->setPalette(pale);
	this->friendHeader->setFixedSize(300, 500);
	this->friendHeader->setScaledContents(true);
	QPixmap pixmap = RoundImage::SquareImageFromPath("D:\\Doga\\WallPaper\\b_7ee3bc45e31b578427bd985ae04f0ab9.png");
	this->friendHeader->setPixmap(pixmap.scaled(QSize(this->friendHeader->size() * GLOB_ScaleDpi), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

FriendInfo::~FriendInfo()
{
}