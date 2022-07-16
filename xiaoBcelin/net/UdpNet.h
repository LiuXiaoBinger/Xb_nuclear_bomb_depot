#pragma once
#include"INet.h"
//class INetMediator;//直接声明

class UdpNet :public INet {
public:
	UdpNet(INetMediator*pMediator);
	~UdpNet();//使用时父类指针指向子类对象，需要使用虚析构防止内存泄漏
	//初始化网络
	virtual bool InitNet();
	//关闭网络
	virtual void UninitNet();
	//发送数据
	virtual bool SendData(long iSendIp, char* buf, int nlen);
protected:
	//接收数据
	virtual void RecData();
	static unsigned int _stdcall RecvThread(void* lpvoid); 
	SOCKET m_sock;
	HANDLE m_hThreadHendle;
	bool m_isStop;
};