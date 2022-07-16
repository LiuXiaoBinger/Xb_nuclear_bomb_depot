#pragma once
#include"INet.h"
#include<map>
#include<list>
//class INetMediator;//直接声明

class TcpServerNet :public INet {
public:
	TcpServerNet(INetMediator* pMediator);
	~TcpServerNet();//使用时父类指针指向子类对象，需要使用虚析构防止内存泄漏
	//初始化网络
	virtual bool InitNet();
	//关闭网络
	virtual void UninitNet();
	//发送数据
	virtual bool SendData(long iSendIp, char* buf, int nlen);
protected:
	//接收数据
	virtual void RecData();
	//接收数据的线程函数
	static unsigned int _stdcall RecvThread(void* lpvoid);
	//接收连接的线程函数
	static unsigned int _stdcall AcceptThread(void* lpvoid);
	map<unsigned int, SOCKET>m_mapThreadldToSocket;
	list < HANDLE> m_hThreaHeadleList;
	SOCKET m_sock;
	HANDLE m_hThreadHendle;
	bool m_isStop;
};