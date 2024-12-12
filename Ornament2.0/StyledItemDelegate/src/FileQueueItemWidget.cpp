#include "../include/FileQueueItemWidget.h"

FileQueueItemWidget::FileQueueItemWidget(const FILEQUEUEITEMDATA& item_data, QWidget* parent)
	: QWidget(parent)
{
	//this->setFixedHeight(50);
	this->m_itemData = item_data;

	QPixmap pixmap;

	this->fileIco = new QLabel(this);
	this->fileIco->setFixedSize(30, 30);
	this->fileIco->setScaledContents(true);
	if (this->m_itemData.FileInfo.FileType == FILETYPE::EXE)
		pixmap.load(":/Resource/ico/exe.png");
	if (this->m_itemData.FileInfo.FileType == FILETYPE::MUSIC)
		pixmap.load(":/Resource/ico/mp3.png");
	if (this->m_itemData.FileInfo.FileType == FILETYPE::PHOTO)
		pixmap.load(":/Resource/ico/photo.png");
	this->fileIco->setPixmap(pixmap);

	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);

	QVBoxLayout* vbox = new QVBoxLayout();

	this->fileName = new QLabel(this->m_itemData.FileInfo.fileName, this);
	this->fileName->adjustSize();

	this->fileSize = new QLabel(this->m_itemData.FileInfo.fileSize, this);
	this->fileSize->adjustSize();

	vbox->addWidget(this->fileName);
	vbox->addWidget(this->fileSize);

	main_lay->addWidget(this->fileIco);
	main_lay->addLayout(vbox);

	this->adjustSize();
}

FileQueueItemWidget::~FileQueueItemWidget()
{
}