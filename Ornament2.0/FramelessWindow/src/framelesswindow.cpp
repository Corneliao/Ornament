#include "../include/framelesswindow.h"

FramelessWindow::FramelessWindow(QWidget* parent)
	: QWidget{ parent }
	, m_titleBar(Q_NULLPTR)
	, m_borderWidth(5)
	, m_justMaximized(false)
	, m_ReSizeable(true)
{
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	this->setResieable(this->m_ReSizeable);
}

void FramelessWindow::setResieable(bool resizeable)
{
	this->m_ReSizeable = resizeable;
	if (this->m_ReSizeable) {
		HWND hwnd = reinterpret_cast<HWND>(this->winId());

		const LONG style = (WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN);
		SetWindowLongPtr(hwnd, GWL_STYLE, style);

		const MARGINS shadow = { 1, 1, 1, 1 };
		DwmExtendFrameIntoClientArea(hwnd, &shadow);

		SetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
	}
}

void FramelessWindow::setResizeableAreaWidth(int width)
{
	if (width < 1)
		width = 1;
	this->m_borderWidth = width;
}

void FramelessWindow::setTitleBar(QWidget* titleBar)
{
	this->m_titleBar = titleBar;
	if (!titleBar)
		return;
}

bool FramelessWindow::nativeEvent(const QByteArray& eventType, void* message, qint64* result)
{
	MSG* msg = reinterpret_cast<MSG*>(message);
	switch (msg->message) {
	case WM_NCCALCSIZE: {
		NCCALCSIZE_PARAMS& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
		if (params.rgrc[0].top != 0)
			params.rgrc[0].top -= 1;

		// cancel  window frame
		*result = WVR_REDRAW;
		return true;
	}
	case WM_NCHITTEST: {
		*result = 0;
		const LONG border_width = this->m_borderWidth;
		RECT winrect;
		GetWindowRect(HWND(this->winId()), &winrect);

		long x = GET_X_LPARAM(msg->lParam);
		long y = GET_Y_LPARAM(msg->lParam);

		if (m_ReSizeable) {
			bool resizeWidth = this->minimumWidth() != this->maximumWidth();
			bool resizeHieght = this->minimumHeight() != this->maximumHeight();

			if (resizeWidth) {
				if (x > winrect.left && x < winrect.left + border_width)
					*result = HTLEFT;
				if (x < winrect.right && x >= winrect.right - border_width)
					*result = HTRIGHT;
			}
			if (resizeHieght) {
				if (y < winrect.top + border_width && y >= winrect.top)
					*result = HTTOP;

				if (y <= winrect.bottom && y > winrect.bottom - border_width)
					*result = HTBOTTOM;
			}
			if (resizeWidth && resizeHieght) {
				// topleft corner
				if (x >= winrect.left && x < winrect.left + border_width && y >= winrect.top && y < winrect.top + border_width) {
					*result = HTTOPLEFT;
				}
				// topRight corner
				if (x <= winrect.right && x > winrect.right - border_width && y >= winrect.top && y < winrect.top + border_width)
					*result = HTTOPRIGHT;
				// leftBottom  corner
				if (x >= winrect.left && x < winrect.left + border_width && y <= winrect.bottom && y > winrect.bottom - border_width)
					*result = HTBOTTOMLEFT;
				// rightbottom  corner
				if (x <= winrect.right && x > winrect.right - border_width && y <= winrect.bottom && y > winrect.bottom - border_width)
					*result = HTBOTTOMRIGHT;
			}
		}
		if (0 != *result)
			return true;
		if (!this->m_titleBar)
			return false;
		// support highdpi
		double dpr = this->devicePixelRatioF();
		QPoint pos = m_titleBar->mapFromGlobal(QPoint(x / dpr, y / dpr));
		if (!this->m_titleBar->rect().contains(pos))
			return false;
		QWidget* child = this->m_titleBar->childAt(pos);
		if (!child) {
			*result = HTCAPTION;
			return true;
		}

		return false;
	}
	case WM_GETMINMAXINFO: {
		if (::IsZoomed(msg->hwnd)) {
			RECT frame = { 0, 0, 0, 0 };
			AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);
			double dpr = this->devicePixelRatioF();
			this->m_frames.setLeft(abs(frame.left) / dpr + 0.5);
			this->m_frames.setTop(abs(frame.bottom) / dpr + 0.5);
			this->m_frames.setRight(abs(frame.right) / dpr + 0.5);
			this->m_frames.setBottom(abs(frame.bottom) / dpr + 0.5);
			QWidget::setContentsMargins(m_frames.left() + m_Margins.left(), m_frames.top() + m_Margins.top(), m_frames.right() + m_Margins.right(), m_frames.bottom() + m_Margins.bottom());
			this->m_justMaximized = true;
		}
		else {
			if (this->m_justMaximized) {
				QWidget::setContentsMargins(this->m_Margins);
				this->m_frames = QMargins();
				this->m_justMaximized = false;
			}
		}
		return false;
	}
	default:
		return QWidget::nativeEvent(eventType, message, result);
	}
}

//void FramelessWindow::showEvent(QShowEvent*)
//{
//	HWND hCurWnd = GetForegroundWindow();
//	DWORD dwMyID = GetCurrentThreadId();
//	DWORD dwCurID = GetWindowThreadProcessId(hCurWnd, NULL);
//
//	AttachThreadInput(dwCurID, dwMyID, TRUE);
//	SetForegroundWindow((HWND)this->winId());
//	AttachThreadInput(dwCurID, dwMyID, FALSE);
//
//	this->show();
//	SwitchToThisWindow((HWND)this->winId(), TRUE);
//	SetActiveWindow((HWND)this->winId());
//}

FranelessWindowForDialog::FranelessWindowForDialog(QWidget* parent) :
	QDialog{ parent }
	, m_titleBar(Q_NULLPTR)
	, m_borderWidth(5)
	, m_justMaximized(false)
	, m_ReSizeable(true)
{
	this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	this->setResieable(this->m_ReSizeable);
}

void FranelessWindowForDialog::setResieable(bool resizeable)
{
	this->m_ReSizeable = resizeable;
	if (this->m_ReSizeable) {
		HWND hwnd = reinterpret_cast<HWND>(this->winId());

		const LONG style = (WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN);
		SetWindowLongPtr(hwnd, GWL_STYLE, style);

		const MARGINS shadow = { 1, 1, 1, 1 };
		DwmExtendFrameIntoClientArea(hwnd, &shadow);

		SetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
	}
}

void FranelessWindowForDialog::setResizeableAreaWidth(int width)
{
	if (width < 1)
		width = 1;
	this->m_borderWidth = width;
}

void FranelessWindowForDialog::setTitleBar(QWidget* titleBar)
{
	this->m_titleBar = titleBar;
	if (!titleBar)
		return;
}

bool FranelessWindowForDialog::nativeEvent(const QByteArray& eventType, void* message, qint64* result)
{
	MSG* msg = reinterpret_cast<MSG*>(message);
	switch (msg->message) {
	case WM_NCCALCSIZE: {
		NCCALCSIZE_PARAMS& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
		if (params.rgrc[0].top != 0)
			params.rgrc[0].top -= 1;

		// cancel  window frame
		*result = WVR_REDRAW;
		return true;
	}
	case WM_NCHITTEST: {
		*result = 0;
		const LONG border_width = this->m_borderWidth;
		RECT winrect;
		GetWindowRect(HWND(this->winId()), &winrect);

		long x = GET_X_LPARAM(msg->lParam);
		long y = GET_Y_LPARAM(msg->lParam);

		if (m_ReSizeable) {
			bool resizeWidth = this->minimumWidth() != this->maximumWidth();
			bool resizeHieght = this->minimumHeight() != this->maximumHeight();

			if (resizeWidth) {
				if (x > winrect.left && x < winrect.left + border_width)
					*result = HTLEFT;
				if (x < winrect.right && x >= winrect.right - border_width)
					*result = HTRIGHT;
			}
			if (resizeHieght) {
				if (y < winrect.top + border_width && y >= winrect.top)
					*result = HTTOP;

				if (y <= winrect.bottom && y > winrect.bottom - border_width)
					*result = HTBOTTOM;
			}
			if (resizeWidth && resizeHieght) {
				// topleft corner
				if (x >= winrect.left && x < winrect.left + border_width && y >= winrect.top && y < winrect.top + border_width) {
					*result = HTTOPLEFT;
				}
				// topRight corner
				if (x <= winrect.right && x > winrect.right - border_width && y >= winrect.top && y < winrect.top + border_width)
					*result = HTTOPRIGHT;
				// leftBottom  corner
				if (x >= winrect.left && x < winrect.left + border_width && y <= winrect.bottom && y > winrect.bottom - border_width)
					*result = HTBOTTOMLEFT;
				// rightbottom  corner
				if (x <= winrect.right && x > winrect.right - border_width && y <= winrect.bottom && y > winrect.bottom - border_width)
					*result = HTBOTTOMRIGHT;
			}
		}
		if (0 != *result)
			return true;
		if (!this->m_titleBar)
			return false;
		// support highdpi
		double dpr = this->devicePixelRatioF();
		QPoint pos = m_titleBar->mapFromGlobal(QPoint(x / dpr, y / dpr));
		if (!this->m_titleBar->rect().contains(pos))
			return false;
		QWidget* child = this->m_titleBar->childAt(pos);
		if (!child) {
			*result = HTCAPTION;
			return true;
		}

		return false;
	}
	case WM_GETMINMAXINFO: {
		if (::IsZoomed(msg->hwnd)) {
			RECT frame = { 0, 0, 0, 0 };
			AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);
			double dpr = this->devicePixelRatioF();
			this->m_frames.setLeft(abs(frame.left) / dpr + 0.5);
			this->m_frames.setTop(abs(frame.bottom) / dpr + 0.5);
			this->m_frames.setRight(abs(frame.right) / dpr + 0.5);
			this->m_frames.setBottom(abs(frame.bottom) / dpr + 0.5);
			//	QDialog::setContentsMargins(m_frames.left() + m_Margins.left(), m_frames.top() + m_Margins.top(), m_frames.right() + m_Margins.right(), m_frames.bottom() + m_Margins.bottom());
			this->m_justMaximized = true;
		}
		else {
			if (this->m_justMaximized) {
				//QDialog::setContentsMargins(this->m_Margins);
				this->m_frames = QMargins();
				this->m_justMaximized = false;
			}
		}
		return false;
	}
	default:
		return QDialog::nativeEvent(eventType, message, result);
	}
}