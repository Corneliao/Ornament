#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QPainter>
#include "../../global.h"
class LoginTitleBar : public QWidget
{
	Q_OBJECT

public:
	LoginTitleBar(QWidget* parent);
	~LoginTitleBar();
protected:
	bool eventFilter(QObject* target, QEvent* event)Q_DECL_OVERRIDE;
private:
	QLabel* close_window = Q_NULLPTR;
signals:
	void closeWindowSignal();
};
