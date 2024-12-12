#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "../../global.h"
class FileQueueItemWidget : public QWidget
{
	Q_OBJECT

public:
	FileQueueItemWidget(const FILEQUEUEITEMDATA& item_data, QWidget* parent);
	~FileQueueItemWidget();
private:
	FILEQUEUEITEMDATA m_itemData;
	QLabel* fileIco = Q_NULLPTR;
	QLabel* fileName = Q_NULLPTR;
	QLabel* fileSize = Q_NULLPTR;
};
