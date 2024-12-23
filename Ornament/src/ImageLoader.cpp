//
// Created by Flache on 2024/12/21.
//

#include <utility>

#include "../include/ImageLoader.h"
ImageLoader::ImageLoader(QQuickItem *parent) : QQuickPaintedItem(parent) {
    this->setAntialiasing(true);
    this->setMipmap(true);
}
void ImageLoader::paint(QPainter *painter) {
    if (painter == nullptr)
        return;
    if(this->m_isFromData) {
        this->image = this->RoundImageFromData(this->m_imageData,this->m_isRoundImage);
    }
    else {
        this->image = this->RoundImage(this->m_imageUrl,this->m_isRoundImage);
    }
    if(this->image.isNull()){
        return;
    }
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->setRenderHints(QPainter::SmoothPixmapTransform);
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPixmap(0, 0, this->image.scaled(QSize(this->m_imageHeight * this->m_dpi, this->m_imageWidth * this->m_dpi), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
void ImageLoader::setImageUrl(QString url_) {
    this->m_imageUrl = std::move(url_);
    if (image.isNull())
        return;
}


void ImageLoader::setImageWidth(int width) {
    this->m_imageWidth = width;
}


void ImageLoader::setImageHeight(int height) {
    this->m_imageHeight = height;
}


QPixmap ImageLoader::RoundImage(const QString &imagePath,bool isRoundImage) const {
    QPixmap pixmap(imagePath);
    QSize realSize = QSize(static_cast<int>(pixmap.width() * this->m_dpi), static_cast<int>(pixmap.height() * this->m_dpi));

    QPixmap resultPixmap(realSize);
    resultPixmap.fill(Qt::transparent);
    QPainter painter(&resultPixmap);
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);
    QPainterPath path;
    if(m_isRoundImage)  {
        path.addRoundedRect(QRect(QPoint(0, 0), QSize(realSize)), qreal(realSize.width() /2), qreal(realSize.height() /2));
    }
    else if(this->m_isRoundBorder) {
        path.addRoundedRect(QRect(QPoint(0, 0), QSize(realSize)), qreal(realSize.width() / m_radius * 2), qreal(realSize.height() / this->m_radius) * 2);
    }
    else  {
        path.addRoundedRect(QRect(QPoint(0, 0), QSize(realSize)), 0,0);
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

QPixmap ImageLoader::RoundImageFromData(const QString &imageData, bool isRoundImage) const{
    QPixmap pixmap;
    pixmap.loadFromData(QByteArray::fromBase64(imageData.toLocal8Bit()));
    if(pixmap.isNull())
        return {};
    QSize realSize = QSize(pixmap.width(), pixmap.height());
    QPixmap resultPixmap(realSize);
    resultPixmap.fill(Qt::transparent);
    QPainter painter(&resultPixmap);
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);
    QPainterPath path;
    if(m_isRoundImage)  {
        path.addRoundedRect(QRect(QPoint(0, 0), QSize(realSize)), static_cast<qreal>(realSize.width())/2, qreal(static_cast<qreal>(realSize.height())/2));
    }
    else if(this->m_isRoundBorder) {
        path.addRoundedRect(QRect(QPoint(0, 0), QSize(realSize)), qreal(realSize.width() / m_radius * 2), qreal(realSize.height() / this->m_radius) * 2);
    }
    else  {
        path.addRoundedRect(QRect(QPoint(0, 0), QSize(realSize)), 0,0);
    }
    painter.setClipPath(path);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPixmap(QRect(QPoint(0, 0), QSize(realSize)), pixmap.scaled(QSize(realSize), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    resultPixmap.setDevicePixelRatio(this->m_dpi);
    return resultPixmap;
}
void ImageLoader::setImageByteArray(const QString &imageData) {
    this->m_imageData = imageData;
}

void ImageLoader::setIsFromData(bool isFromData) {
    this->m_isFromData = isFromData;
    this->update();
}

void ImageLoader::setIsRoundBorder(bool isRoundBorder) {
    this->m_isRoundBorder = isRoundBorder;
}

void ImageLoader::setIsRoundImage(bool isRound) {
    this->m_isRoundImage = isRound;
}

bool ImageLoader::isRoundImage() const {
    return this->m_isRoundImage;
}

QString ImageLoader::imageUrl() const {
    return this->m_imageUrl;
}
int ImageLoader::imageWidth() const {
    return this->m_imageWidth;
}
int ImageLoader::imageHeight() const {
    return this->m_imageHeight;
}
qreal ImageLoader::windowDpi() const {
    return this->m_dpi;
}
QString ImageLoader::imageByteArray() const {
    return this->m_imageData;
}
bool ImageLoader::isFromData() const {
    return this->m_isFromData;
}
bool ImageLoader::isRoundBorder() const {
    return this->m_isRoundBorder;
}

