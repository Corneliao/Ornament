#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "../../Component/include/RoundImage.h"
#include "../../global.h"
class FriendInfo : public QWidget
{
	Q_OBJECT

public:
	FriendInfo(QWidget* parent);
	~FriendInfo();
private:
	QLabel* friendHeader = Q_NULLPTR;
};
