#include "../include/smoothlistwidget.h"

SmoothScrollListWidgetBar::SmoothScrollListWidgetBar(QWidget* parent)
  : QScrollBar(parent)
{
    this->m_targetValue_v = this->value();
    this->timeLineAnimation = new QTimeLine(1000, this);
    this->timeLineAnimation->setUpdateInterval(0);
    this->timeLineAnimation->setEasingCurve(QEasingCurve::OutBack);
    connect(this->timeLineAnimation, &QTimeLine::frameChanged, this, &SmoothScrollListWidgetBar::updateValue, Qt::DirectConnection);
}

void SmoothScrollListWidgetBar::startSetValue(int value)
{
    this->timeLineAnimation->stop();
    this->timeLineAnimation->setFrameRange(this->value(), value);
    this->timeLineAnimation->start();
}
void SmoothScrollListWidgetBar::scroll(int value)
{
    m_targetValue_v -= value; // 将目标值和相对位置进行运算
    if (this->m_targetValue_v < 0) {
        this->m_targetValue_v = 0;
    }
    if (this->m_targetValue_v > this->maximum()) {
        this->m_targetValue_v = this->maximum();
    }
    startSetValue(m_targetValue_v); // 开始动画
}

void SmoothScrollListWidgetBar::updateValue(const int& value)
{
    this->setValue(value);
}

void SmoothScrollListWidgetBar::mousePressEvent(QMouseEvent* e)
{
    this->timeLineAnimation->stop();
    m_targetValue_v = value();
    QScrollBar::mousePressEvent(e);
}
void SmoothScrollListWidgetBar::mouseReleaseEvent(QMouseEvent* e)
{
    this->timeLineAnimation->stop();
    m_targetValue_v = value();
    QScrollBar::mouseReleaseEvent(e);
}
void SmoothScrollListWidgetBar::mouseMoveEvent(QMouseEvent* e)
{
    this->timeLineAnimation->stop();
    m_targetValue_v = value();
    QScrollBar::mouseMoveEvent(e);
}

SmoothListWidget::SmoothListWidget(QWidget* parent)
  : QListWidget(parent)
{
    QPalette pale;
    pale.setColor(QPalette::Window, Qt::transparent);
    this->scrollBar = new SmoothScrollListWidgetBar(this);

    this->setAutoFillBackground(true);
    this->setPalette(pale);
    this->setFrameShape(QFrame::NoFrame);
    this->setVerticalScrollBar(this->scrollBar);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setUniformItemSizes(true);
}

void SmoothListWidget::wheelEvent(QWheelEvent* e)
{
    this->scrollBar->scroll(e->angleDelta().y());
    e->accept();
}
