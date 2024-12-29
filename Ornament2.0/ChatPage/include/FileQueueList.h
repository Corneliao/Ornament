#pragma once

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPainter>
#include "../../StyledItemDelegate/include/FileQueueItemWidget.h"
#include "../../global.h"
class FileQueueList : public QWidget
{
	Q_OBJECT

public:
	FileQueueList(QWidget* parent);
	~FileQueueList();
	void increaseFileTask(const FILEQUEUEITEMDATA & file_data);
protected:
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
private:
	QListWidget* FileList = Q_NULLPTR;
};
