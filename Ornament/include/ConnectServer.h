//
// Created by Flache on 2024/12/22.
//

#ifndef ORNAMENT_CONNECTSERVER_H
#define ORNAMENT_CONNECTSERVER_H


#include <QTcpSocket>
#include <QObject>
class ConnectServer:public QObject {
    Q_OBJECT
    enum MSGTYPE {TEXT = 0};
public:
    explicit ConnectServer(QObject * parent = Q_NULLPTR);
    ~ConnectServer()override;

    void initializeSocket();

    void sendUserTextMessage(const QString & senderUser,const QString & recverUser,const QString & messagetext);

private:
    void ReceiveSocketData();
private:
    QTcpSocket * socket = Q_NULLPTR;
};


#endif//ORNAMENT_CONNECTSERVER_H
