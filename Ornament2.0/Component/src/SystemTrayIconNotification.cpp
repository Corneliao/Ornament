#include "../include/SystemTrayIconNotification.h"

SystemTrayIconNotification* SystemTrayIconNotification::trayicon_instence = Q_NULLPTR;
SystemTrayIconNotification::AutoRelease release;

SystemTrayIconNotification* SystemTrayIconNotification::getInstence()
{
	if (trayicon_instence == Q_NULLPTR) {
		trayicon_instence = new SystemTrayIconNotification();
		trayicon_instence = trayicon_instence;
	}
	return trayicon_instence;
}

void SystemTrayIconNotification::showMessage(const QString& title, const QString& text, const QPixmap& icon)
{
	//QSystemTrayIcon::MessageIcon megIcon = QSystemTrayIcon::MessageIcon();
	this->trayIcon->showMessage(title, text, icon);
}
SystemTrayIconNotification::SystemTrayIconNotification()
{
	this->trayIcon = new QSystemTrayIcon(this);
	this->trayIcon->setToolTip("Ornament");
	this->trayIcon->setIcon(QIcon(":/Resource/ico/TablerBrandUnity.png"));
	this->trayIcon->show();
}

SystemTrayIconNotification::~SystemTrayIconNotification()
{
}