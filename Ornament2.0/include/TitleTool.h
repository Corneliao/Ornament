#pragma once

#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>
#include <windows.h>
class Tool;
class TitleTool : public QWidget
{
	Q_OBJECT

public:
	TitleTool(QWidget* parent);
	~TitleTool();
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	QGraphicsDropShadowEffect* shadow = Q_NULLPTR;
	Tool* tool = Q_NULLPTR;
signals:
	void closeWindowSignal();
	void minWindowSignal();
	void maxWindowSignal();
};

class Tool :public QWidget {
	Q_OBJECT
public:
	explicit Tool(QWidget* parent = Q_NULLPTR);
protected:
	bool eventFilter(QObject* target, QEvent* event)Q_DECL_OVERRIDE;
private:
	QLabel* minWindow = Q_NULLPTR;
	QLabel* maxWindow = Q_NULLPTR;
	QLabel* closeWindow = Q_NULLPTR;
signals:
	void closeWindowSignal();
	void minWindowSignal();
	void maxWindowSignal();
};
