#include "../include/FileTcpServer.h"

FileTcpServer::FileTcpServer(QObject* parent)
	: QTcpServer(parent)
{
	if (this->listen(QHostAddress::Any, quint16(7028))) {
		qDebug() << "文件传输服务已启动";
	}
}

FileTcpServer::~FileTcpServer()
{
}

void FileTcpServer::incomingConnection(const qintptr handle)
{
	emit this->newFileClientWork(handle);
}
