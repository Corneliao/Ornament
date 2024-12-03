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
	this->userHead->setFixedSize(35, 35);
	this->userHead->setScaledContents(true);
	this->userHead->setPixmap(this->user_data.userHead.scaled(QSize(this->userHead->size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));;

	QVBoxLayout* vbox = new QVBoxLayout;
	this->userName = new QLabel(this->user_data.userName);
	this->userName->adjustSize();
	this->bubble = new Bubble(user_data, this);

	if (this->user_data.alignment == Qt::AlignLeft) {
		vbox->addWidget(this->userName, 0, Qt::AlignLeft);
		vbox->addWidget(this->bubble);
		//vbox->addSpacing(3);

		main_lay->addWidget(this->userHead, 0, Qt::AlignTop);
		main_lay->addLayout(vbox);
		main_lay->addStretch();
	}
	else {
		vbox->addWidget(this->userName, 0, Qt::AlignRight);
		vbox->addWidget(this->bubble);
		vbox->addSpacing(3);

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
	if (position >= 1)
		this->user_data.fileInfo.isUploading = false;
}

void MessageItemWidget::paintEvent(QPaintEvent*)
{
	//QPainter painter(this);
	//painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);

	//QLabel temp(user_data.userName);
	//temp.adjustSize();

	////painter->save();
	////painter->setPen(Qt::NoPen);
	////painter->setBrush(Qt::red);
	////painter->drawRoundedRect(item_rect, 10, 10);
	////painter->restore();

	//if (user_data.messageType == ChatMessageType::TEXT) {
	//	QRect userhead_rect;
	//	QRect username_rect;
	//	QRect bubble_rect;
	//	if (user_data.alignment == Qt::AlignLeft) {
	//		//头像
	//		userhead_rect = QRect(this->rect().left() + 3, this->rect().top() + 5, 40, 40);
	//		//昵称
	//		username_rect = QRect(QPoint(userhead_rect.right() + 5, userhead_rect.top()), QSize(temp.size()));

	//		//绘制气泡
	//		temp.setText(user_data.userMessage);
	//		temp.adjustSize();
	//		bubble_rect = QRect(username_rect.left(), username_rect.bottom() + 5, temp.width() + 20, temp.height() + 10);
	//	}
	//	else if (user_data.alignment == Qt::AlignRight) {
	//		//头像
	//		userhead_rect = QRect(this->rect().right() - (40), this->rect().top() + 5, 40, 40);
	//		//昵称
	//		username_rect = QRect(QPoint(userhead_rect.left() - (temp.width() + 5), userhead_rect.top()), QSize(temp.size()));

	//		//绘制气泡
	//		temp.setText(user_data.userMessage);
	//		temp.adjustSize();
	//		bubble_rect = QRect(username_rect.right() - (temp.width() + 20), username_rect.bottom() + 5, temp.width() + 20, temp.height() + 10);
	//	}

	//	//绘制头像
	//	painter.save();
	//	painter.setPen(Qt::NoPen);
	//	painter.setBrush(Qt::NoBrush);
	//	painter.drawPixmap(userhead_rect, user_data.userHead.scaled(QSize(40 * GLOB_ScaleDpi, 40 * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	//	painter.restore();

	//	//绘制昵称
	//	QFont font;
	//	font.setPixelSize(10);
	//	font.setBold(true);
	//	painter.save();
	//	painter.setPen(Qt::black);
	//	painter.setFont(font);
	//	painter.setBrush(Qt::NoBrush);
	//	painter.drawText(username_rect, Qt::AlignCenter, user_data.userName);
	//	painter.restore();

	//	QRect message_rect;
	//	message_rect.setSize(temp.size());
	//	message_rect.moveCenter(bubble_rect.center());

	//	painter.save();
	//	if (user_data.alignment == Qt::AlignLeft) {
	//		painter.setPen(Qt::NoPen);
	//		painter.setBrush(QColor::fromString("#6a4cff"));
	//		painter.drawRoundedRect(bubble_rect, 10, 10);
	//	}
	//	else {
	//		painter.setPen(Qt::NoPen);
	//		painter.setBrush(QColor::fromString("#0099ff"));
	//		painter.drawRoundedRect(bubble_rect, 10, 10);
	//	}
	//	painter.restore();

	//	painter.save();
	//	painter.setPen(Qt::white);
	//	painter.setBrush(Qt::NoBrush);
	//	painter.drawText(message_rect, Qt::AlignLeft, user_data.userMessage);
	//	painter.restore();
	//}
	//else if (user_data.messageType == ChatMessageType::USERFILE) {
	//	QRect userhead_rect;
	//	QRect username_rect;
	//	QLabel  fileTemp(user_data.fileInfo.fileName);
	//	fileTemp.adjustSize();

	//	const int IcoWidth = 40;
	//	QRect bubble_rect;
	//	if (user_data.alignment == Qt::AlignLeft) {
	//		//头像
	//		userhead_rect = QRect(this->rect().left() + 3, this->rect().top() + 3, 40, 40);
	//		//昵称
	//		username_rect = QRect(QPoint(userhead_rect.right() + 5, userhead_rect.top()), QSize(temp.size()));

	//		bubble_rect = QRect(username_rect.left(), username_rect.bottom() + 5, fileTemp.width() + IcoWidth + 20, IcoWidth + 20);
	//	}
	//	else if (user_data.alignment == Qt::AlignRight) {
	//		//头像
	//		userhead_rect = QRect(this->rect().right() - (40), this->rect().top() + 3, 40, 40);
	//		//昵称
	//		username_rect = QRect(QPoint(userhead_rect.left() - (temp.width() + 5), userhead_rect.top()), QSize(temp.size()));

	//		bubble_rect = QRect(username_rect.right() - (fileTemp.width() + IcoWidth + 20), username_rect.bottom() + 5, fileTemp.width() + IcoWidth + 20, IcoWidth + 20);
	//	}

	//	painter.save();
	//	painter.setPen(Qt::NoPen);
	//	painter.setBrush(QColor::fromString("#0099ff"));
	//	painter.drawRoundedRect(bubble_rect, 10, 10);
	//	painter.restore();

	//	//绘制头像
	//	painter.save();
	//	painter.setPen(Qt::NoPen);
	//	painter.setBrush(Qt::NoBrush);
	//	painter.drawPixmap(userhead_rect, user_data.userHead.scaled(QSize(40 * GLOB_ScaleDpi, 40 * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	//	painter.restore();

	//	//绘制昵称
	//	QFont font;
	//	font.setPixelSize(10);
	//	font.setBold(true);
	//	painter.save();
	//	painter.setPen(Qt::black);
	//	painter.setFont(font);
	//	painter.setBrush(Qt::NoBrush);
	//	painter.drawText(username_rect, Qt::AlignCenter, user_data.userName);
	//	painter.restore();

	//	QRect fileIco_rect = QRect(bubble_rect.left() + 5, bubble_rect.top() + 5, IcoWidth, IcoWidth);
	//	if (!user_data.fileInfo.fileIco.isNull()) {
	//		fileIco_rect = QRect(bubble_rect.left() + 5, bubble_rect.top() + 5, IcoWidth, IcoWidth);
	//		painter.save();
	//		painter.setPen(Qt::NoPen);
	//		painter.setBrush(Qt::NoBrush);
	//		painter.drawPixmap(fileIco_rect, user_data.fileInfo.fileIco.scaled(QSize(40 * GLOB_ScaleDpi, 40 * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	//		painter.restore();
	//	}

	//	QRect fileName_rect(fileIco_rect.right() + 5, fileIco_rect.top() + 3, fileTemp.width(), fileTemp.height());
	//	painter.save();
	//	painter.setPen(Qt::white);
	//	painter.setBrush(Qt::NoBrush);
	//	painter.drawText(fileName_rect, Qt::AlignCenter, fileTemp.text());
	//	painter.restore();

	//	fileTemp.setText(user_data.fileInfo.fileSize);
	//	fileTemp.adjustSize();
	//	QRect fileSize_rect(fileName_rect.left(), fileName_rect.bottom() + 5, fileTemp.width(), fileTemp.height());
	//	painter.save();
	//	painter.setPen(Qt::white);
	//	painter.setBrush(Qt::NoBrush);
	//	painter.drawText(fileSize_rect, Qt::AlignCenter, fileTemp.text());
	//	painter.restore();

	//	QRect sliderBackground_rect(bubble_rect.center().x() - ((bubble_rect.width() - 20) / 2), bubble_rect.bottom() - 10, bubble_rect.width() - 20, 5);
	//	//绘制进度条
	//	painter.save();
	//	painter.setPen(Qt::NoPen);
	//	painter.setBrush(Qt::gray);
	//	painter.drawRoundedRect(sliderBackground_rect, 3, 3);
	//	painter.restore();

	//	painter.save();
	//	painter.setPen(Qt::NoPen);
	//	painter.setBrush(Qt::green);
	//	painter.drawRoundedRect(QRect(QPoint(sliderBackground_rect.topLeft()), QSize(sliderBackground_rect.width() * user_data.fileInfo.position, 5)), 3, 3);
	//	painter.restore();
	//}
}

Bubble::Bubble(const UserData& user_data, QWidget* parent) :QWidget(parent)
{
	QPalette pale;
	pale.setColor(QPalette::Text, Qt::white);
	QFont font;
	if (user_data.messageType == ChatMessageType::TEXT) {
		this->text = new QLabel(user_data.userMessage, this);
		this->text->setPalette(pale);
		this->text->adjustSize();
		this->setFixedSize(text->width() + 20, text->height() + 10);
		this->text->setGeometry(QRect(QPoint(this->rect().center().x() - (this->text->width() / 2), this->rect().center().y() - (this->text->height() / 2)), QSize(this->text->size())));
	}
	else if (user_data.messageType == ChatMessageType::USERFILE) {
		QPixmap  pixmap;
		if (user_data.fileInfo.FileType == FILETYPE::EXE) {
			QVBoxLayout* main_vbox = new QVBoxLayout(this);
			//main_vbox->setContentsMargins(0, 0, 0, 0);
			this->setLayout(main_vbox);

			pixmap.load(":/Resource/ico/exe.png");
			this->fileIco = new QLabel(this);
			this->fileIco->setFixedSize(40, 40);
			this->fileIco->setScaledContents(true);
			this->fileIco->setPixmap(pixmap.scaled(QSize(this->fileIco->size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

			this->fileName = new QLabel(user_data.fileInfo.fileName, this);
			this->fileName->setPalette(pale);
			this->fileName->adjustSize();

			font.setPixelSize(11);
			this->fileSize = new QLabel(user_data.fileInfo.fileSize, this);
			this->fileSize->setFont(font);
			this->fileSize->setPalette(pale);
			this->fileSize->adjustSize();
			this->setFixedSize(this->fileName->width() + this->fileIco->width() + 30, this->fileIco->width() + 30);

			this->slider = new ProgressSlider(this->width() - 30, this);
			QHBoxLayout* lay = new QHBoxLayout;

			QVBoxLayout* vbox = new QVBoxLayout;
			vbox->addWidget(this->fileName);
			vbox->addWidget(this->fileSize);

			lay->addWidget(this->fileIco);
			lay->addLayout(vbox);

			main_vbox->addLayout(lay);
			main_vbox->addSpacing(5);
			main_vbox->addWidget(this->slider);
		}
	}
}

void Bubble::setSliderPosition(const qreal& position)
{
	this->slider->setSliderPosition(position);
}

void Bubble::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor::fromString("#0099ff"));
	painter.drawRoundedRect(this->rect(), 10, 10);
}

ProgressSlider::ProgressSlider(int width, QWidget* parent) :QWidget(parent)
{
	this->setFixedSize(width, 5);
}

void ProgressSlider::setSliderPosition(const qreal& position)
{
	this->m_position = position;
	this->update();
}

void ProgressSlider::paintEvent(QPaintEvent*)
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
}
