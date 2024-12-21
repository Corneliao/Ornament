//
// Created by Flache on 2024/12/21.
//

#include "../include/ImageLoader.h"
ImageLoader::ImageLoader(QQuickItem *parent) : QQuickPaintedItem(parent) {
    this->setAntialiasing(true);
    this->setMipmap(true);
}
void ImageLoader::paint(QPainter *painter) {
    if (painter == nullptr)
        return;


    this->image = this->RoundImage(this->m_imageUrl,this->m_isRoundImage);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPixmap(0, 0, this->image.scaled(QSize(this->m_imageHeight * this->m_dpi, this->m_imageWidth * this->m_dpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}
void ImageLoader::setImageUrl(QString url) {
    this->m_imageUrl = url;
    if (image.isNull())
        return;
}
void ImageLoader::setImageWidth(int width) {
    this->m_imageWidth = width;
}
void ImageLoader::setImageHeight(int height) {
    this->m_imageHeight = height;
}
QString ImageLoader::getImageUrl() const {
    return this->m_imageUrl;
}
int ImageLoader::getImageHeight() const {
    return this->m_imageHeight;
}
int ImageLoader::getImageWidth() const {
    return this->m_imageWidth;
}
QPixmap ImageLoader::RoundImage(const QString &imagePath,bool isRoundImage) {


    QPixmap pixmap(imagePath);
    QSize realSize = QSize(pixmap.width(), pixmap.height());

    QPixmap resultPixmap(realSize);
    resultPixmap.fill(Qt::transparent);
    QPainter painter(&resultPixmap);
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);
    QPainterPath path;
    if(isRoundImage)  {
        path.addRoundedRect(QRect(QPoint(0, 0), QSize(realSize)), qreal(realSize.width() /2), qreal(realSize.height() /2));
    }
    else {
        path.addRoundedRect(QRect(QPoint(0, 0), QSize(realSize)), qreal(realSize.width() / m_radius * 2), qreal(realSize.height() / this->m_radius) * 2);
    }

    painter.setClipPath(path);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPixmap(QRect(QPoint(0, 0), QSize(realSize)), pixmap.scaled(QSize(realSize), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    resultPixmap.setDevicePixelRatio(this->m_dpi);

    return resultPixmap;
}
void ImageLoader::setRadius(qreal radius) {
    this->m_radius = radius;
}
void ImageLoader::setWindowDpi(qreal dpi) {
    this->m_dpi = dpi;
}
qreal ImageLoader::getWindowDpi() const {
    return this->m_dpi;
}
void ImageLoader::setRoundImage(bool isRound) {
    this->m_isRoundImage =isRound;
}
bool ImageLoader::getRoundImage() const {
    return this->m_isRoundImage;
}
