#pragma once

#include <QWidget>
#include "../FramelessWindow/include/framelesswindow.h"
class AddFriend : public FramelessWindow
{
	Q_OBJECT

public:
	AddFriend(QWidget* parent = Q_NULLPTR);
	~AddFriend();
private:
};
