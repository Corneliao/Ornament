#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPainter>
class ButtonComponent : public QWidget
{
	Q_OBJECT

public:
	ButtonComponent(const QString& text, QWidget* parent);
	~ButtonComponent();
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	QLabel* button_text = Q_NULLPTR;
	QColor backgroundColor = QColor::fromString("#0099ff");
};
