#pragma once

#include <QPixmap>
#include <QPainterPath>
#include <QPainter>
#include "../../global.h"
#include <QDebug>
#include <QPixmap>
#include <QImageReader>
#include <QImage>

class RoundImage
{
public:
	static QPixmap RoundImageFromStringPath(const QString& imagePath);
	static QPixmap RoundImageFromByteArray(const QByteArray& data);
	static QPixmap SquareImageFromPath(const QString& imagePath);
	RoundImage();;
	~RoundImage();
};
