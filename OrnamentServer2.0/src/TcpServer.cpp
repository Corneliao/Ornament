#include "../include/TcpServer.h"

TcpServer::TcpServer(QObject* parent)
	:QTcpServer(parent)
{
	if (!this->listen(QHostAddress::Any, quint16(7502)))
		return;
	qDebug() << "监听成功";
}

TcpServer::~TcpServer()
{
	if (this->isListening())
		this->close();
	
}

void TcpServer::incomingConnection(qintptr handle)
{
	emit this->newClientConnected(handle);
}
