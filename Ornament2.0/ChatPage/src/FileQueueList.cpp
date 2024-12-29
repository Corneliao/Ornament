#include "../include/FileQueueList.h"

FileQueueList::FileQueueList(QWidget* parent)
	: QWidget(parent)
{
	this->setFixedWidth(200);
	QVBoxLayout* vbox = new QVBoxLayout(this);
	vbox->setContentsMargins(0, 0, 0, 0);
	this->setLayout(vbox);

	this->FileList = new QListWidget(this);
	this->FileList->setFrameShape(QFrame::NoFrame);
	QStringList qss;
	qss.append("QListWidget::item:hover{background:transparent}");
	qss.append("QListWidget{background-color:transparent}");
	this->FileList->setStyleSheet(qss.join(""));
	vbox->addWidget(this->FileList, 0, Qt::AlignCenter);


	vbox->addStretch();


	this->adjustSize();
}

FileQueueList::~FileQueueList()
{
}

void FileQueueList::increaseFileTask(const FILEQUEUEITEMDATA& file_data)
{
	QListWidgetItem* item = new QListWidgetItem(this->FileList);
	item->setData(Qt::UserRole, QVariant::fromValue(file_data));
	item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
	FileQueueItemWidget* itemWidget = new FileQueueItemWidget(file_data, this->FileList);
	item->setSizeHint(itemWidget->size());
	this->FileList->addItem(item);
	this->FileList->setItemWidget(item, itemWidget);
}

void FileQueueList::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::transparent);
	painter.drawRoundedRect(this->rect(), 8.0f, 8.0f);

}
