#include "../include/ImageViewer.h"

ImageViewer::ImageViewer(const QString& image_path, QWidget* parent)
	: FramelessWindow(parent)
{
	QFileInfo info(image_path);
	this->fileName = info.fileName();
	this->setWindowTitle("图片查看");
	this->setMinimumSize(640, 480);
	QVBoxLayout* vbox = new QVBoxLayout(this);
	vbox->setContentsMargins(0, 0, 0, 0);
	this->setLayout(vbox);

	this->view = new CustomGraphicsView(this);
	this->view->setBackgroundBrush(Qt::black);

	this->view->setRenderHints(QPainter::Antialiasing);
	this->view->setRenderHints(QPainter::SmoothPixmapTransform);

	this->scene = new QGraphicsScene(this);
	//this->scene->setBackgroundBrush(Qt::white);
	this->view->setScene(this->scene);
	this->pixmapItem = new QGraphicsPixmapItem();
	this->pixmapItem->setTransformationMode(Qt::SmoothTransformation);
	this->m_image.load(image_path);
	this->m_image = this->m_image.scaled(QSize(this->m_image.size() * GLOB_ScaleDpi), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	this->view->setSceneRect(this->m_image.rect());
	this->pixmapItem->setPixmap(QPixmap::fromImage(this->m_image));
	this->scene->addItem(this->pixmapItem);

	this->title = new  ImageViewerTitle(this);
	this->setTitleBar(this->title);

	this->image_tool = new ImageTool(this);

	vbox->addWidget(this->title);
	vbox->addWidget(this->view);
	vbox->addWidget(this->image_tool);

	connect(this->image_tool, &ImageTool::on_save_image, this, &ImageViewer::SaveImage, Qt::DirectConnection);
	connect(this->image_tool, &ImageTool::on_rotation_image, this, &ImageViewer::RotationImage, Qt::DirectConnection);
	connect(this->title, &ImageViewerTitle::on_close_window, this, &ImageViewer::close,Qt::DirectConnection);
}
ImageViewer::~ImageViewer()
{
}

/**
 * @brief 点击图片消息时设置当前图片
 * @param image_path
 */
void ImageViewer::setPixmap(const QString& image_path)
{
	this->m_image.load(image_path);
	this->m_image = this->m_image.scaled(QSize(this->m_image.size() * GLOB_ScaleDpi), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	this->view->setSceneRect(this->m_image.rect());
	this->pixmapItem->setPixmap(QPixmap::fromImage(this->m_image));
	this->view->fitInView(this->pixmapItem, Qt::KeepAspectRatio);
	this->move((screenSize.width() - this->width()) / 2, (screenSize.height() - this->height()) / 2);
}

void ImageViewer::resizeEvent(QResizeEvent* event) {
	this->view->fitInView(this->pixmapItem, Qt::KeepAspectRatio);
	QWidget::resizeEvent(event);
}

void ImageViewer::showEvent(QShowEvent* event)
{
	this->view->fitInView(this->pixmapItem, Qt::KeepAspectRatio);
	this->move((screenSize.width() - this->width()) / 2, (screenSize.height() - this->height()) / 2);
	QWidget::showEvent(event);
}

void ImageViewer::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::black);
	painter.drawRoundedRect(this->rect(), 8.5f, 8.5f);
	QWidget::paintEvent(event);
}

void ImageViewer::closeEvent(QCloseEvent* event)
{
	pixmapItem->setTransform(QTransform());
	this->transform_image = this->pixmapItem->pixmap().transformed(this->pixmapItem->transform(), Qt::SmoothTransformation).toImage();
}

void ImageViewer::SaveImage()
{
	if (this->transform_image.isNull())
		return;

	//QString save_path = QFileDialog::getSaveFileName(this, "保存图片",this->fileName);
	QString save_path = QFileDialog::getExistingDirectory(this, "保存图片", "/");
	if (!save_path.isEmpty()) {
		this->transform_image.save(save_path + "/" + this->fileName);
		qDebug() << "保存成功";
	}
}

void ImageViewer::RotationImage(bool direction)
{
	if (direction) {
		// 设置图片项的变换原点为中心
		pixmapItem->setTransformOriginPoint(this->m_image.width() / static_cast<qreal>(2), this->m_image.height() / static_cast<qreal>(2));

		// 创建旋转变换
		QTransform transform;
		transform.translate(this->m_image.width() / static_cast<qreal>(2), this->m_image.height() / static_cast<qreal>(2)); // 移动到变换原点
		transform.rotate(this->getRotationAngle(this->pixmapItem->transform()) + 90);
		transform.translate(-this->m_image.width() / static_cast<qreal>(2), -this->m_image.height() / static_cast<qreal>(2)); // 移回到原始位置

		// 应用变换
		pixmapItem->setTransform(transform);
		this->view->fitInView(this->pixmapItem, Qt::KeepAspectRatio);
		this->transform_image = this->pixmapItem->pixmap().transformed(transform, Qt::SmoothTransformation).toImage();
	}
	else {
		pixmapItem->setTransformOriginPoint(this->m_image.width() / static_cast<qreal>(2), this->m_image.height() / static_cast<qreal>(2));

		// 创建旋转变换
		QTransform transform;
		transform.translate(this->m_image.width() / static_cast<qreal>(2), this->m_image.height() / static_cast<qreal>(2)); // 移动到变换原点
		transform.rotate(this->getRotationAngle(this->pixmapItem->transform()) - 90);
		transform.translate(-this->m_image.width() / static_cast<qreal>(2), -this->m_image.height() / static_cast<qreal>(2)); // 移回到原始位置

		// 应用变换
		pixmapItem->setTransform(transform);
		//// 居中图片项
		this->view->fitInView(this->pixmapItem, Qt::KeepAspectRatio);
		this->transform_image = this->pixmapItem->pixmap().transformed(transform, Qt::SmoothTransformation).toImage();
	}
}
qreal ImageViewer::getRotationAngle(const QTransform& transform)
{
	// 从变换矩阵中提取旋转角度
// 注意：这个方法仅适用于纯旋转矩阵或者旋转是最后一个操作的情况
	qreal theta = atan2(transform.m12(), transform.m11());
	return theta * 180.0 / M_PI; // 将弧度转换为度
}
ImageTool::ImageTool(QWidget* parent) :QWidget(parent)
{
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);

	QPixmap pixmap;
	this->saveImage = new QLabel(this);
	this->saveImage->setCursor(Qt::PointingHandCursor);
	this->saveImage->setFixedSize(25, 25);
	this->saveImage->setScaledContents(true);
	this->saveImage->installEventFilter(this);
	pixmap.load(":/Resource/ico/save_image.png");
	pixmap = pixmap.scaled(QSize(this->saveImage->size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	this->saveImage->setPixmap(pixmap);

	this->turnLeft = new QLabel(this);
	this->turnLeft->setFixedSize(this->saveImage->size());
	this->turnLeft->setScaledContents(true);
	this->turnLeft->installEventFilter(this);
	this->turnLeft->setCursor(Qt::PointingHandCursor);
	pixmap.load(":/Resource/ico/LsiconRotationLeftOutline.png");
	pixmap = pixmap.scaled(QSize(this->turnLeft->size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	this->turnLeft->setPixmap(pixmap);

	this->turnRight = new QLabel(this);
	this->turnRight->setFixedSize(this->turnLeft->size());
	this->turnRight->setScaledContents(true);
	this->turnRight->setCursor(Qt::PointingHandCursor);
	this->turnRight->installEventFilter(this);
	pixmap.load(":/Resource/ico/LsiconRotationRightOutline.png");
	pixmap = pixmap.scaled(QSize(this->turnRight->size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	this->turnRight->setPixmap(pixmap);

	main_lay->addWidget(this->turnLeft);
	main_lay->addWidget(this->turnRight);
	main_lay->addWidget(this->saveImage);

}

bool ImageTool::eventFilter(QObject* target, QEvent* event)
{
	if (target == this->saveImage) {
		if (event->type() == QEvent::MouseButtonPress) {
			emit this->on_save_image();
			return true;
		}
	}
	if (target == this->turnLeft) {
		if (event->type() == QEvent::MouseButtonPress) {
			emit this->on_rotation_image(false);
			return true;
		}
	}
	if (target == this->turnRight) {
		if (event->type() == QEvent::MouseButtonPress) {
			emit this->on_rotation_image(true);
			return true;
		}
	}
	return QWidget::eventFilter(target, event);
}

CustomGraphicsView::CustomGraphicsView(QWidget* parent)
{
	this->setFrameShape(QFrame::NoFrame);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setViewportMargins(0, 0, 0, 0);
	this->setResizeAnchor(QGraphicsView::NoAnchor);
	this->setCacheMode(QGraphicsView::CacheBackground);
	this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	this->setDragMode(QGraphicsView::ScrollHandDrag);
	this->setOptimizationFlags(QGraphicsView::DontSavePainterState);

}

void CustomGraphicsView::wheelEvent(QWheelEvent* event)
{
	QPointF scenePos = mapToScene(event->position().toPoint());

	//// 获取滚轮的滚动量
	double angle = event->angleDelta().y();
	double factor = qPow(1.0015, angle); // 或者使用其他缩放因子
	//// 设置视图的缩放

	this->scale(factor, factor);

	QPointF viewPos = mapFromScene(scenePos);
	horizontalScrollBar()->setValue(viewPos.x() - event->position().x());
	verticalScrollBar()->setValue(viewPos.y() - event->position().y());
	QGraphicsView::wheelEvent(event);
}

ImageViewerTitle::ImageViewerTitle(QWidget* parent) :QWidget(parent)
{
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);

	QPalette pale;
	pale.setColor(QPalette::WindowText, Qt::white);
	this->title_text = new QLabel("图片查看", this);
	this->title_text->setPalette(pale);
	this->title_text->adjustSize();

	this->close_window = new QLabel(this);
	this->close_window->setCursor(Qt::PointingHandCursor);
	this->close_window->setFixedSize(25, 25);
	this->close_window->setScaledContents(true);
	this->close_window->installEventFilter(this);
	QPixmap pixmap(":/Resource/ico/IconamoonClose.png");
	pixmap = pixmap.scaled(QSize(this->close_window->size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	this->close_window->setPixmap(pixmap);

	main_lay->addWidget(this->title_text);
	main_lay->addStretch();
	main_lay->addWidget(this->close_window);
}

bool ImageViewerTitle::eventFilter(QObject* target, QEvent* event)
{
	if (target == this->close_window) {
		if (event->type() == QEvent::MouseButtonPress) {
			emit this->on_close_window();
			return true;
		}
	}
	return QWidget::eventFilter(target, event);
}
