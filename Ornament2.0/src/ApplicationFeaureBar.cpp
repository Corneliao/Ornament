#include "../include/ApplicationFeaureBar.h"

ApplicationFeaureBar::ApplicationFeaureBar(QWidget* parent)
	: QWidget(parent)
{
	this->setFixedWidth(60);
	this->main_vbox = new QVBoxLayout(this);
	main_vbox->setContentsMargins(0, 11, 0, 11);
	main_vbox->setSpacing(10);
	this->setLayout(main_vbox);

	this->increaseFeature(QString(":/Resource/ico/message_unselected.png"), QString(":/Resource/ico/message_selected.png"), "消息");
	this->increaseFeature(QString(":/Resource/ico/friend_unselected.png"), QString(":/Resource/ico/friend_selected.png"), QString("好友"));
	main_vbox->addStretch();
}

ApplicationFeaureBar::~ApplicationFeaureBar()
{
}
void ApplicationFeaureBar::paintEvent(QPaintEvent*)
{
	//QPainter painter(this);
	//painter.setRenderHint(QPainter::Antialiasing);
	//painter.setPen(Qt::NoPen);
	//painter.setBrush(QColor::fromString("#6a4cff"));
	//painter.drawRoundedRect(this->rect(), 15, 15);
}

void ApplicationFeaureBar::increaseFeature(const QString& normal_ico, const QString& select_ico, const QString& ico_text)
{
	FeatureButton* button = new FeatureButton(normal_ico, select_ico, ico_text, this->itemIndex, this);
	this->main_vbox->addWidget(button, 0, Qt::AlignCenter);
	//this->main_vbox->addSpacing(15);
	itemIndex++;
	connect(button, &FeatureButton::clicked, this, &ApplicationFeaureBar::dealFeatureButtonClicked, Qt::DirectConnection);
}

void ApplicationFeaureBar::dealFeatureButtonClicked(const int index)
{
	if (index <= -1)
		return;
	emit this->indexChanged(index);
	for (int i = 0; i < this->main_vbox->count(); i++) {
		QLayoutItem* item = this->main_vbox->itemAt(i);
		FeatureButton* widget = qobject_cast<FeatureButton*>(item->widget());
		if (widget != Q_NULLPTR) {
			if (widget->GetCurrentIndex() != index) {
				widget->setUnSelected();
			}
		}
	}
}

void ApplicationFeaureBar::setCurrentFeatureButton(const int index)
{
	for (int i = 0; i < this->main_vbox->count(); i++) {
		QLayoutItem* item = this->main_vbox->itemAt(i);
		FeatureButton* widget = qobject_cast<FeatureButton*>(item->widget());
		if (widget != Q_NULLPTR) {
			if (widget->GetCurrentIndex() == index) {
				widget->setSelected();
			}
			else {
				widget->setUnSelected();
			}
		}
	}
}

FeatureButton::FeatureButton(const QString& normaIco, const QString& selectedIco, const QString& buttonText, const int index, QWidget* parent) :QWidget(parent)
{
	this->currentIndex = index;
	if (this->currentIndex == 0)
		this->isPressed = true;
	//	this->ico_text = buttonText;
	this->setFixedSize(parent->width() - 20, parent->width() - 20);
	this->setCursor(Qt::PointingHandCursor);
	this->normalPixmap.load(normaIco);
	this->selectedPixmap.load(selectedIco);
}

int FeatureButton::GetCurrentIndex() const
{
	return this->currentIndex;
}

void FeatureButton::setUnSelected()
{
	this->isPressed = false;
	this->update();
}

void FeatureButton::setSelected()
{
	this->isPressed = true;
	this->update();
}

void FeatureButton::enterEvent(QEnterEvent*)
{
	this->isHover = true;
	this->update();
}

void FeatureButton::leaveEvent(QEvent* e)
{
	this->isHover = false;
	this->update();
}

void FeatureButton::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	QFont font;
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	if (this->isPressed || this->isHover) {
		painter.save();
		painter.setPen(Qt::NoPen);
		painter.setBrush(QColor(0, 0, 0, 20));
		painter.drawRoundedRect(QRect(this->rect()), 15, 15);
		//painter
		painter.restore();

		painter.save();
		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::NoBrush);
		//painter.drawPixmap(QRect(this->rect().left() + 5, this->rect().center().y() - (25 / 2), 25, 25), this->selectedPixmap.scaled(QSize(25, 25), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		painter.drawPixmap(QRect(this->rect().center().x() - (18 / 2), this->rect().center().y() - (18 / 2), 18, 18), this->selectedPixmap.scaled(QSize(25, 25), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		painter.restore();

		//font.setBold(true);
		//painter.save();
		//painter.setFont(font);
		//painter.setPen(QColor::fromString("#2775fe"));
		//painter.setBrush(Qt::NoBrush);
		//painter.drawText(this->rect(), Qt::AlignCenter, this->ico_text);
		//painter.restore();
	}
	else {
		painter.save();
		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::NoBrush);
		//painter.drawPixmap(QRect(this->rect().left() + 5, this->rect().center().y() - (25 / 2), 25, 25), this->normalPixmap.scaled(QSize(25, 25), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		painter.drawPixmap(QRect(this->rect().center().x() - (18 / 2), this->rect().center().y() - (18 / 2), 18, 18), this->normalPixmap.scaled(QSize(25, 25), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

		painter.restore();

		//painter.save();
		//painter.setPen(QColor::fromString("#999999"));
		//painter.setBrush(Qt::NoBrush);
		//painter.drawText(this->rect(), Qt::AlignCenter, this->ico_text);
		//painter.restore();
	}
}

void FeatureButton::mousePressEvent(QMouseEvent*)
{
	//QPalette pale;
	//pale.setColor(QPalette::WindowText, Qt::black);
	//QFont font;
	//font.setBold(true);
	//this->ico_text->setFont(font);
	//this->ico_text->adjustSize();
	if (this->isPressed)
		return;
	this->isPressed = true;
	this->update();
	emit this->clicked(this->currentIndex);
}
