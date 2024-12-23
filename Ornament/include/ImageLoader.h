//
// Created by Flache on 2024/12/21.
//

#ifndef ORNAMENT_IMAGELOADER_H
#define ORNAMENT_IMAGELOADER_H
#include <QObject>
#include <QPainter>
#include <QPainterPath>
#include <QQuickPaintedItem>
class ImageLoader : public QQuickPaintedItem {
    Q_OBJECT
    QML_ELEMENT

    // Prperties
    Q_PROPERTY(QString imageUrl WRITE setImageUrl READ imageUrl NOTIFY imageUrlChanged);
    Q_PROPERTY(int imageWidth WRITE setImageWidth READ imageWidth);
    Q_PROPERTY(int imageHeight WRITE setImageHeight READ imageHeight);
    Q_PROPERTY(qreal radius WRITE setRadius);
    Q_PROPERTY(qreal windowDpi WRITE setWindowDpi READ windowDpi);
    Q_PROPERTY(bool isRoundImage WRITE setIsRoundImage READ isRoundImage);
    Q_PROPERTY(QString imageByteArray WRITE setImageByteArray READ imageByteArray NOTIFY imageByteArrayChanged)
    Q_PROPERTY(bool isFromData WRITE setIsFromData READ  isFromData NOTIFY isFromDataChanged)
    Q_PROPERTY(bool isRoundBorder WRITE setIsRoundBorder READ isRoundBorder NOTIFY isRoundBorderChanged)


public:
    explicit ImageLoader(QQuickItem *parent = Q_NULLPTR);
    ~ImageLoader()override = default;
    void paint(QPainter *painter) override;
    void setImageUrl(QString url_);
    [[nodiscard]] QString imageUrl() const;
    void setImageWidth(int width);
    [[nodiscard]] int imageWidth() const;
    void setImageHeight(int height);
    [[nodiscard]] int imageHeight() const;
    void setRadius(qreal radius);
    void setWindowDpi(qreal dpi);
    [[nodiscard]] qreal windowDpi() const;
    void  setIsRoundImage(bool  isRound);
    [[nodiscard]] bool  isRoundImage()const;
    void setImageByteArray(const QString &imageData);
    [[nodiscard]] QString imageByteArray()const;
    void setIsFromData(bool isFromData);
    [[nodiscard]] bool isFromData()const;
    void setIsRoundBorder(bool  isRoundBorder);
    [[nodiscard]] bool isRoundBorder()const;

private:
    [[nodiscard]] QPixmap RoundImage(const QString &imagePath,bool isRoundImage) const;
    [[nodiscard]] QPixmap RoundImageFromData(const QString &imageData, bool isRoundImage) const;
    QPixmap image;
    QString m_imageUrl;
    int m_imageWidth = 0;
    int m_imageHeight = 0;
    qreal m_radius = 0.0;
    qreal m_dpi = 0.0;
    bool  m_isRoundImage = false;
    QString m_imageData;
    bool  m_isFromData = false;
    bool  m_isRoundBorder =false;
Q_SIGNALS:
    void imageUrlChanged();
    void imageByteArrayChanged();
    void isFromDataChanged();
    void isRoundBorderChanged();
};


#endif//ORNAMENT_IMAGELOADER_H
