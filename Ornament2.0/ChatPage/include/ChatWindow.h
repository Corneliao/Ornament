#pragma once

#include <QWidget>

class ChatWindow : public QWidget
{
	Q_OBJECT

public:
	ChatWindow(QWidget* parent);
	~ChatWindow();
};
