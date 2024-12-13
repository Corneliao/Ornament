#pragma once

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QWheelEvent>
#include <QScrollBar>
#include <QPropertyAnimation>
#include "../../FramelessWindow/include/framelesswindow.h"
#include "../../global.h"

class ImageViewerTitle;
class CustomGraphicsView;
class ImageTool;
QT_FORWARD_DECLARE_CLASS(ImageTool);
QT_FORWARD_DECLARE_CLASS(CustomGraphicsView);
QT_FORWARD_DECLARE_CLASS(ImageViewerTitle);

class ImageViewer : public FramelessWindow
{
	Q_OBJECT

public:
	ImageViewer(const QString& image_path, QWidget* parent);
	~ImageViewer();
	void setPixmap(const  QString& image_path);

protected:
	void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
	void showEvent(QShowEvent* event) Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
	void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;
private:
	void SaveImage();
	void RotationImage(bool direction);
	qreal getRotationAngle(const QTransform& transform);
private:
	QGraphicsPixmapItem* pixmapItem = Q_NULLPTR;
	QGraphicsScene* scene = Q_NULLPTR;
	QImage m_image;
	CustomGraphicsView* view = Q_NULLPTR;
	ImageTool* image_tool = Q_NULLPTR;
	QString fileName;
	QImage transform_image;
	ImageViewerTitle* title = Q_NULLPTR;
};

class CustomGraphicsView :public  QGraphicsView {
	Q_OBJECT
		Q_PROPERTY(double zoom READ zoom WRITE setZoom)
public:
	CustomGraphicsView(QWidget* parent = Q_NULLPTR);
	double zoom() const;
	void setZoom(double newZoom);
protected:
	void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;
private:
	QPropertyAnimation* scaleAnimation = Q_NULLPTR;
	double m_zoom = 1.0;
};

class ImageTool :public QWidget {
	Q_OBJECT
public:
	explicit ImageTool(QWidget* parent = Q_NULLPTR);
protected:
	bool eventFilter(QObject* target, QEvent* event)Q_DECL_OVERRIDE;
private:
	QLabel* saveImage = Q_NULLPTR;
	QLabel* turnLeft = Q_NULLPTR;
	QLabel* turnRight = Q_NULLPTR;
Q_SIGNALS:
	void on_save_image();
	void on_rotation_image(bool direction);
};

class ImageViewerTitle :public  QWidget {
public:
	explicit ImageViewerTitle(QWidget* parent = Q_NULLPTR);
private:
	QLabel* close_window = Q_NULLPTR;
	QLabel* title_text = Q_NULLPTR;
};
