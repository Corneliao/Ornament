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
    Q_PROPERTY(QString imageUrl WRITE setImageUrl READ getImageUrl NOTIFY imageUrlChanged);
    Q_PROPERTY(int imageWidth WRITE setImageWidth READ getImageWidth);
    Q_PROPERTY(int imageHeight WRITE setImageHeight READ getImageHeight);
    Q_PROPERTY(qreal radius WRITE setRadius);
    Q_PROPERTY(qreal windowDpi WRITE setWindowDpi READ getWindowDpi);
    Q_PROPERTY(bool isRoundImage WRITE setRoundImage READ getRoundImage);
    QML_ELEMENT

public:
    explicit ImageLoader(QQuickItem *parent = Q_NULLPTR);
    ~ImageLoader() = default;
    void paint(QPainter *painter) override;
    void setImageUrl(QString url);
    QString getImageUrl() const;
    void setImageWidth(int width);
    void setImageHeight(int height);
    int getImageWidth() const;
    int getImageHeight() const;
    void setRadius(qreal radius);
    void setWindowDpi(qreal dpi);
    qreal getWindowDpi() const;
    void  setRoundImage(bool  isRound);
    bool  getRoundImage()const;

private:
    QPixmap RoundImage(const QString &imagePath,bool isRoundImage);
    QPixmap image;
    QString m_imageUrl;
    int m_imageWidth = 0;
    int m_imageHeight = 0;
    qreal m_radius = 0.0;
    qreal m_dpi = 0.0;
    bool  m_isRoundImage = false;

Q_SIGNALS:
    void imageUrlChanged();
};


#endif//ORNAMENT_IMAGELOADER_H
