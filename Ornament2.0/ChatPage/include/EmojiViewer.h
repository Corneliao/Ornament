#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QTextImageFormat>
#include <QTextCursor>
#include <QGraphicsDropShadowEffect>
#include "../../global.h"

class  EmojiLabel;
QT_FORWARD_DECLARE_CLASS(EmojiLabel);

class EmojiViewer : public QWidget
{
	Q_OBJECT

public:
	EmojiViewer(QWidget* parent);
	~EmojiViewer();
	void addEmojiItem(const QString& emoji_path);
protected:
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
private:
	QGridLayout* main_grid = Q_NULLPTR;
	QGraphicsDropShadowEffect* shadow = Q_NULLPTR;
	int curRow = 0;
	int curColum = 0;
Q_SIGNALS:
	void emoji_pixmap(const QString& image_path);
};

class EmojiLabel :public QLabel {
	Q_OBJECT
public:
	EmojiLabel(const QString& image_path, QWidget* parent = Q_NULLPTR);
protected:
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
private:
	QString m_image_path;
Q_SIGNALS:
	void emoji_pixmap(const QString& image_path);
};
