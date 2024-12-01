#pragma once

#include <QObject>
#include <QSystemTrayIcon>
#include <QDebug>

class SystemTrayIconNotification : public QObject
{
	Q_OBJECT

public:
	static SystemTrayIconNotification* getInstence();
	void showMessage(const QString& title, const QString& text, const QPixmap& icon);
	class AutoRelease {
	public:
		~AutoRelease() {
			if (trayicon_instence) {
				trayicon_instence->deleteLater();
				trayicon_instence = Q_NULLPTR;
			}
		}
	};
	static AutoRelease release;
private:
	SystemTrayIconNotification();
	~SystemTrayIconNotification();
private:
	static SystemTrayIconNotification* trayicon_instence;
	QSystemTrayIcon* trayIcon = Q_NULLPTR;
};
