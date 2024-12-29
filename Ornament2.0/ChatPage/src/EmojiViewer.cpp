#include "../include/EmojiViewer.h"

EmojiViewer::EmojiViewer(QWidget* parent)
	: QWidget(parent)
{
	//this->setFixedSize(200, 200);
	this->main_grid = new QGridLayout(this);
	//this->main_grid->setContentsMargins(0, 0, 0, 0);

	this->setLayout(this->main_grid);

	this->addEmojiItem(":/Resource/emoji/a-aimujingpeijingyangwasejingyabiaoqingxiaolian.png");
	this->addEmojiItem(":/Resource/emoji/a-aiqingxihuansebiaoqingxiaolian.png");
	this->addEmojiItem(":/Resource/emoji/a-aixihuanliukoushuisebiaoqingxiaolian.png");
	this->addEmojiItem(":/Resource/emoji/a-baochitaiduobiaoqingxiaolian.png");
	this->addEmojiItem(":/Resource/emoji/a-bizuigangabiaoqingxiaolian.png");
	this->addEmojiItem(":/Resource/emoji/a-gangaliuhanbiaoqingxiaolian.png");

	this->adjustSize();

	this->shadow = new QGraphicsDropShadowEffect(this);
	this->shadow->setOffset(0, 0);
	this->shadow->setBlurRadius(15);
	this->shadow->setColor(Qt::lightGray);
	this->setGraphicsEffect(this->shadow);
}

EmojiViewer::~EmojiViewer()
{
}

void EmojiViewer::addEmojiItem(const QString& emoji_path)
{
	EmojiLabel* emoji_label = new EmojiLabel(emoji_path, this);
	emoji_label->setCursor(Qt::PointingHandCursor);
	emoji_label->setFixedSize(20, 20);
	emoji_label->setScaledContents(true);
	QPixmap pixmap(emoji_path);
	pixmap = pixmap.scaled(emoji_label->size() * GLOB_ScaleDpi, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	emoji_label->setPixmap(pixmap);
	this->main_grid->addWidget(emoji_label, this->curRow, this->curColum);
	this->curColum++;
	if (this->curColum >= 3) {
		this->curColum = 0;
		this->curRow++;
	}
	connect(emoji_label, &EmojiLabel::emoji_pixmap, this, &EmojiViewer::emoji_pixmap, Qt::DirectConnection);
}

void EmojiViewer::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::white);
	painter.drawRoundedRect(this->rect(), 8.5f, 8.5f);
}

EmojiLabel::EmojiLabel(const QString& image_path, QWidget* parent) :QLabel(parent)
{
	this->m_image_path = image_path;
}

void EmojiLabel::mousePressEvent(QMouseEvent* event)
{
	emit this->emoji_pixmap(this->m_image_path);
}