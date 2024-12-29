#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QWidget>
#include <QDialog>
#include <dwmapi.h>
#include <WinUser.h>
#include <windowsx.h>
#include <QDialog>
#include <QEvent>
constexpr int RESIZE_WINDOW_WIDTH = 8;
#pragma comment(lib,"dwmapi.lib")
class FramelessWindow : public QWidget {
	Q_OBJECT
public:
	explicit FramelessWindow(QWidget* parent = nullptr);

protected:
	// 设置一个标题栏，widget
	void setTitleBar(QWidget* titleBar);
	bool nativeEvent(const QByteArray& eventType, void* message, qint64* result) override;
	bool event(QEvent* event) Q_DECL_OVERRIDE;

private:

	int adjustResizeWindow(HWND hwnd, const QPoint& pos);
	QWidget* m_titleBar = Q_NULLPTR;
	bool m_isMaximized = false;
};

class FranelessWindowForDialog :public QDialog {
public:
	explicit FranelessWindowForDialog(QWidget* parent = Q_NULLPTR);

protected:
	// 设置一个标题栏，widget
	void setTitleBar(QWidget* titleBar);
	bool nativeEvent(const QByteArray& eventType, void* message, qint64* result) override;
private:

	int adjustResizeWindow(HWND hwnd, const QPoint& pos);
	QWidget* m_titleBar = Q_NULLPTR;
	bool m_isMaximized = false;
};
#endif  // FRAMELESSWINDOW_H
