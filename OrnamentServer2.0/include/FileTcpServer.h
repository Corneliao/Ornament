#pragma once

#include <QTcpServer>

class FileTcpServer : public QTcpServer
{
	Q_OBJECT

public:
	FileTcpServer(QObject* parent);
	~FileTcpServer();
protected:
	void incomingConnection(const qintptr handle)Q_DECL_OVERRIDE;
signals:
	void newFileClientWork(const qintptr handle);
};
