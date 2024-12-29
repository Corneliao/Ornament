#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include "../../global.h"
class DefaultWindow : public QWidget
{
	Q_OBJECT

public:
	DefaultWindow(QWidget *parent);
	~DefaultWindow();
private:
	QLabel* NoChatWindowIco = Q_NULLPTR;
};
