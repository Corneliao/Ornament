#include "../include/ApplicationFeaureBar.h"

ApplicationFeaureBar::ApplicationFeaureBar(QWidget* parent)
	: QWidget(parent)
{
	this->setFixedWidth(160);
	this->main_vbox = new QVBoxLayout(this);
	this->setLayout(main_vbox);

	this->addfriend_button = new AddFriendButton(this);
	this->addfriend_button->installEventFilter(this);

	main_vbox->addWidget(this->addfriend_button, 0, Qt::AlignCenter);
	main_vbox->addSpacing(20);

	this->increaseFeature(QString(":/Resource/ico/RiMessage3Line.png"), QString(":/Resource/ico/RiMessage3Fill.png"), "消息");
	this->increaseFeature(QString(":/Resource/ico/RiMessage3Line.png"), QString(":/Resource/ico/RiMessage3Fill.png"), "消息");
	this->increaseFeature(QString(":/Resource/ico/friendIco.png"), QString(":/Resource/ico/friendIco_fill.png"), QString("好友"));
	main_vbox->addStretch();
}

ApplicationFeaureBar::~ApplicationFeaureBar()
{
}
void ApplicationFeaureBar::paintEvent(QPaintEvent*)
{
}

void ApplicationFeaureBar::increaseFeature(const QString& normal_ico, const QString& select_ico, const QString& ico_text)
{
	FeatureButton* button = new FeatureButton(normal_ico, select_ico, ico_text, this->itemIndex, this);
	this->main_vbox->addWidget(button);
	this->main_vbox->addSpacing(15);
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

AddFriendButton::AddFriendButton(QWidget* parent)
{
	this->setCursor(Qt::PointingHandCursor);
	this->setFixedSize(parent->width() - 40, 35);
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);

	this->ico = new QLabel(this);
	this->ico->setFixedSize(15, 15);
	this->ico->setScaledContents(true);
	QPixmap pixmap(":/Resource/ico/invite.png");
	this->ico->setPixmap(pixmap);

	QPalette pale;
	pale.setColor(QPalette::WindowText, QColor::fromString("#2570fb"));
	this->ico_text = new QLabel("添加好友", this);
	this->ico_text->setPalette(pale);
	this->ico->adjustSize();

	main_lay->addWidget(this->ico);
	main_lay->addWidget(this->ico_text, 0, Qt::AlignCenter);
}

void AddFriendButton::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(153, 204, 255, 60));
	painter.drawRoundedRect(this->rect(), 10, 10);
}

FeatureButton::FeatureButton(const QString& normaIco, const QString& selectedIco, const QString& buttonText, const int index, QWidget* parent) :QWidget(parent)
{
	this->currentIndex = index;
	this->ico_text = buttonText;
	this->setFixedSize(parent->width() - 20, 40);
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

void FeatureButton::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	QFont font;
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	if (this->isPressed) {
		painter.save();
		painter.setPen(Qt::NoPen);
		painter.setBrush(QColor::fromString("#2775fe"));
		painter.drawRoundedRect(QRect(this->rect().right() - (5 + 2), this->rect().center().y() - (static_cast<qreal>(30) / 2), 5, 30), static_cast<qreal>(5) / 2, static_cast <qreal>(5) / 2);
		painter.restore();

		painter.save();
		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::NoBrush);
		painter.drawPixmap(QRect(this->rect().left() + 5, this->rect().center().y() - (25 / 2), 25, 25), this->selectedPixmap.scaled(QSize(25, 25), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		painter.restore();

		font.setBold(true);
		painter.save();
		painter.setFont(font);
		painter.setPen(QColor::fromString("#2775fe"));
		painter.setBrush(Qt::NoBrush);
		painter.drawText(this->rect(), Qt::AlignCenter, this->ico_text);
		painter.restore();
	}
	else {
		painter.save();
		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::NoBrush);
		painter.drawPixmap(QRect(this->rect().left() + 5, this->rect().center().y() - (25 / 2), 25, 25), this->normalPixmap.scaled(QSize(25, 25), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		painter.restore();

		painter.save();
		painter.setPen(QColor::fromString("#999999"));
		painter.setBrush(Qt::NoBrush);
		painter.drawText(this->rect(), Qt::AlignCenter, this->ico_text);
		painter.restore();
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
