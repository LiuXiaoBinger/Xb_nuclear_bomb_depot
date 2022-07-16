#pragma once
#include"INetMediator.h"
class TcpClientMediator : public INetMediator {
    Q_OBJECT
public:

	TcpClientMediator();
	~TcpClientMediator();
	//开启网络
	bool OpenNet();
	//关闭网络
	void CloseNet();
	//发送数据
	bool SendData(long lSendlp, char* buf, int nLen);
	//处理数据
	void DealData(long lSendlp, char* buf, int nLen);
signals:
    //发送接收的数据给kernel类
    void SIG_ReadyData(long lSendlp, char* buf, int nLen);

};
