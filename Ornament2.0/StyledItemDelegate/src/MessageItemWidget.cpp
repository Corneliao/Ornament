#include "../include/MessageItemWidget.h"

MessageItemWidget::MessageItemWidget(const UserData& userdata, QWidget* parent)
	: QWidget(parent)
{
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);

	this->user_data = userdata;

	if (this->user_data.messageType == ChatMessageType::TEXT) {
		QLabel temp;
		temp.setText(this->user_data.userMessage);
		temp.adjustSize();
		this->setFixedHeight(50 + temp.height());
	}
	else {
		this->setFixedHeight(110);
	}

	this->userHead = new QLabel(this);
	this->userHead->setFixedSize(36, 36);
	this->userHead->setScaledContents(true);
	this->userHead->setPixmap(this->user_data.userHead.scaled(QSize(this->userHead->size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));;

	QVBoxLayout* vbox = new QVBoxLayout;
	this->userName = new QLabel(this->user_data.userName);
	this->userName->adjustSize();
	this->bubble = new Bubble(user_data, this);

	if (this->user_data.alignment == Qt::AlignLeft) {
		vbox->addWidget(this->userName, 0, Qt::AlignLeft);
		vbox->addWidget(this->bubble, 0, Qt::AlignLeft);
		//vbox->addSpacing(3);

		main_lay->addWidget(this->userHead, 0, Qt::AlignTop);
		main_lay->addSpacing(3);
		main_lay->addLayout(vbox);
		main_lay->addStretch();
	}
	else {
		vbox->addWidget(this->userName, 0, Qt::AlignRight);
		vbox->addWidget(this->bubble, 0, Qt::AlignRight);
		//vbox->addSpacing(3);

		main_lay->addStretch();
		main_lay->addLayout(vbox);
		main_lay->addWidget(this->userHead, 0, Qt::AlignTop);
	}
}

MessageItemWidget::~MessageItemWidget()
{
}

UserData MessageItemWidget::currentMessageItemData() const
{
	return this->user_data;
}

void MessageItemWidget::setSliderPosition(const qreal& position)
{
	this->bubble->setSliderPosition(position);
	if (position >= 1) {
		if (this->user_data.fileInfo.isUploading) {
			this->user_data.fileInfo.isUploading = false;
			this->bubble->setStatus("已发送");
		}
		else if (this->user_data.fileInfo.isDownloading) {
			this->user_data.fileInfo.isDownloading = false;
			this->bubble->setStatus("已接受");
		}
	}
}

void MessageItemWidget::paintEvent(QPaintEvent*)
{
}

Bubble::Bubble(const UserData& user_data, QWidget* parent) :QWidget(parent)
{
	QPalette pale;
	pale.setColor(QPalette::Text, Qt::white);
	QFont font;
	if (user_data.messageType == ChatMessageType::TEXT) {
		QHBoxLayout* main_lay = new QHBoxLayout(this);
		main_lay->setContentsMargins(2, 2, 2, 2);
		this->setLayout(main_lay);
		this->text = new QLabel(user_data.userMessage, this);
		this->text->setPalette(pale);
		this->text->adjustSize();
		this->setFixedSize(text->width() + 20, text->height() + 10);
		main_lay->addWidget(this->text, 0, Qt::AlignCenter);
		this->border_rect = this->rect().adjusted(2, 2, -2, -2);
	}
	else if (user_data.messageType == ChatMessageType::USERFILE) {
		QPixmap  pixmap;
		QVBoxLayout* main_vbox = new QVBoxLayout(this);
		this->setLayout(main_vbox);
		if (user_data.fileInfo.FileType == FILETYPE::EXE) {
			pixmap.load(":/Resource/ico/exe.png");
		}
		else if (user_data.fileInfo.FileType == FILETYPE::MUSIC)
			pixmap.load(":/Resource/ico/mp3.png");

		this->fileIco = new QLabel(this);
		this->fileIco->setFixedSize(40, 40);
		this->fileIco->setScaledContents(true);
		this->fileIco->setPixmap(pixmap.scaled(QSize(this->fileIco->size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

		this->fileName = new QLabel(user_data.fileInfo.fileName, this);
		this->fileName->setPalette(pale);
		this->fileName->adjustSize();

		QHBoxLayout* hlay = new QHBoxLayout;
		font.setPixelSize(11);
		this->fileSize = new QLabel(user_data.fileInfo.fileSize, this);
		this->fileSize->setFont(font);
		this->fileSize->setPalette(pale);
		this->fileSize->adjustSize();

		font.setPixelSize(9);
		this->status = new QLabel(this);
		this->status->setAlignment(Qt::AlignCenter);
		this->status->adjustSize();
		this->status->setPalette(pale);
		this->status->setFont(font);
		if (user_data.fileInfo.isDownloading) {
			this->status->setText("接受中");
		}
		if (user_data.fileInfo.isUploading) {
			this->status->setText("上传中");
		}

		hlay->addWidget(this->fileSize);
		hlay->addStretch();
		hlay->addWidget(this->status);

		this->slider = new ProgressSlider(this);
		QHBoxLayout* lay = new QHBoxLayout;

		QVBoxLayout* vbox = new QVBoxLayout;
		vbox->addWidget(this->fileName, 0, Qt::AlignLeft);
		vbox->addLayout(hlay);

		lay->addWidget(this->fileIco);
		lay->addLayout(vbox);

		main_vbox->addLayout(lay);
		main_vbox->addWidget(this->slider, 0, Qt::AlignCenter);

		this->border_rect = this->rect().adjusted(2, 2, -2, -2);
	}
}

void Bubble::setSliderPosition(const qreal& position)
{
	this->slider->setSliderPosition(position);
}

void Bubble::setStatus(const QString& text)
{
	this->status->setText(text);
	this->status->adjustSize();
}

void Bubble::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor::fromString("#0099ff"));
	painter.drawRoundedRect(this->border_rect, 10, 10);
}

void Bubble::showEvent(QShowEvent* event)
{
	this->border_rect = this->rect().adjusted(2, 2, -2, -2);
	this->update();
	QWidget::showEvent(event);
}

void Bubble::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton) {
	}
}

ProgressSlider::ProgressSlider(QWidget* parent) :QWidget(parent)
{
	this->setFixedHeight(5);
}

void ProgressSlider::setSliderPosition(const qreal& position)
{
	this->m_position = position;
	this->update();
}

void ProgressSlider::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.save();
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::lightGray);
	painter.drawRoundedRect(this->rect(), 3, 3);
	painter.restore();

	painter.save();
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::green);
	painter.drawRoundedRect(QRect(QPoint(this->rect().topLeft()), QSize(this->width() * this->m_position, this->height())), 3, 3);
	painter.restore();
	QWidget::paintEvent(event);
}

void ProgressSlider::showEvent(QShowEvent*)
{
	this->setFixedWidth(this->parentWidget()->width() - 30);
}
