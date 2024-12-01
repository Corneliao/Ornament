#ifndef SMOOTHLISTWIDGET_H
#define SMOOTHLISTWIDGET_H

#include <QListWidget>
#include <QScrollBar>
#include <QTimeLine>
#include <QWheelEvent>
#include <QWidget>
class SmoothScrollListWidgetBar : public QScrollBar {
    Q_OBJECT
    public:
    SmoothScrollListWidgetBar(QWidget *parent = nullptr);

    private:
    // 这里重写鼠标事件的目的是在手动点击或拖动滚动条时更新m_targetValue_v变量，并且在拖动时立即结束滚动的动画。
    // 这里如果不明白作用，可以先注释掉看看手动拖动滚动条时对动画有什么影响。
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    int m_targetValue_v; // 用来记录目标位置的变量
    QTimeLine *timeLineAnimation;
    public slots:
    void startSetValue(int value); // 重写的setValue槽函数，实现动画效果
    void scroll(int value);        // 新增相对滚动的槽函数，value为滚动距离的矢量表示
    void updateValue(const int &value);
    signals:
};

class SmoothListWidget : public QListWidget {
    public:
    SmoothListWidget(QWidget *parent = nullptr);

    protected:
    void wheelEvent(QWheelEvent *e) override;

    private:
    SmoothScrollListWidgetBar *scrollBar;
};

#endif // SMOOTHLISTWIDGET_H
