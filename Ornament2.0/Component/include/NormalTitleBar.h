#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QPainter>
#include "../../global.h"
class NormalTitleBar : public QWidget
{
	Q_OBJECT

public:
	NormalTitleBar(const QString& text = "", QWidget* parent = Q_NULLPTR);
	~NormalTitleBar();
protected:
	bool eventFilter(QObject* target, QEvent* event)Q_DECL_OVERRIDE;
private:
	QLabel* close_window = Q_NULLPTR;
	QLabel* title = Q_NULLPTR;
signals:
	void closeWindowSignal();
};
