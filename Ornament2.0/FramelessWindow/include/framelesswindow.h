#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QWidget>
#include <QDialog>
#include <dwmapi.h>
#include <WinUser.h>
#include <windowsx.h>
#include <QDialog>
#pragma comment(lib,"dwmapi.lib")
class FramelessWindow : public QWidget {
	Q_OBJECT
public:
	explicit FramelessWindow(QWidget* parent = nullptr);
	// 是否可通过鼠标缩放窗口大小
	void setResieable(bool resizeable = true);
	void setResizeableAreaWidth(int width = 5);

protected:
	// 设置一个标题栏，widget
	void setTitleBar(QWidget* titleBar);
	bool nativeEvent(const QByteArray& eventType, void* message, qint64* result) override;
signals:
public slots:

private:
	QWidget* m_titleBar;
	int m_borderWidth;
	QMargins m_Margins;
	bool m_ReSizeable;
	bool m_justMaximized;
	QMargins m_frames;
};

class FranelessWindowForDialog :public QDialog {
public:
	explicit FranelessWindowForDialog(QWidget* parent = Q_NULLPTR);
	void setResieable(bool resizeable = true);
	void setResizeableAreaWidth(int width = 5);

protected:
	// 设置一个标题栏，widget
	void setTitleBar(QWidget* titleBar);
	bool nativeEvent(const QByteArray& eventType, void* message, qint64* result) override;
	//void showEvent(QShowEvent*)override;
signals:
public slots:

private:
	QWidget* m_titleBar;
	int m_borderWidth;
	QMargins m_Margins;
	bool m_ReSizeable;
	bool m_justMaximized;
	QMargins m_frames;
};
#endif  // FRAMELESSWINDOW_H
