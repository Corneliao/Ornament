#include "../include/ButtonComponent.h"

ButtonComponent::ButtonComponent(const QString& text, QWidget* parent)
	: QWidget(parent)
{
	this->setFixedHeight(40);
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);
	QPalette pale;
	pale.setColor(QPalette::WindowText, Qt::white);
	this->button_text = new QLabel(text, this);
	this->button_text->setPalette(pale);
	this->button_text->adjustSize();
	main_lay->addWidget(this->button_text, 0, Qt::AlignCenter);
	this->setCursor(Qt::PointingHandCursor);
}

ButtonComponent::~ButtonComponent()
{
}

void ButtonComponent::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(this->backgroundColor);
	painter.drawRoundedRect(this->rect(), 10, 10);
}
