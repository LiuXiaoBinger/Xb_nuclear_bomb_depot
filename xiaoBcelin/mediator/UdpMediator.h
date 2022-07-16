#pragma once
#include"INetMediator.h"
class UdpMediator : public INetMediator {
public:

	UdpMediator();
	~UdpMediator();
	//开启网络
	bool OpenNet();
	//关闭网络
	void CloseNet();
	//发送数据
	bool SendData(long lSendlp, char* buf, int nLen);
	//处理数据
	void DealData(long lSendlp,char* buf,int nLen);

};

