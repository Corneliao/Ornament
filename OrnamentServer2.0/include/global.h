#pragma once
#include <QString>
#include <QQueue>

#define DEBUGINFO qDebug() << __FUNCTION__ << __TIME__
QT_FORWARD_DECLARE_STRUCT(FILETASKQUEUE);

struct FILETASKQUEUE {
	QString senderUserAccount;
	QString receiveUserAccount;
	QString filepath;
};
