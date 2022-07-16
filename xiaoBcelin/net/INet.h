#pragma once
#include<iostream>
#include<WinSock2.h>
#include<ws2tcpip.h>
#include<process.h>
using namespace std;
//#include<atlbase.h>
//#pragma comment(lib, "ws2_32.lib")
class INetMediator;//直接声明

class INet {
public:
	INet(){}
	virtual~INet() {}//使用时父类指针指向子类对象，需要使用虚析构防止内存泄漏
	//初始化网络
	virtual bool InitNet() = 0;
	//关闭网络
	virtual void UninitNet() = 0;
	//发送数据
	virtual bool SendData(long iSendIp, char* buf, int nlen) = 0;
    static std:: string getIpstring ( long ip){
              sockaddr_in addr;
              addr.sin_addr.S_un.S_addr = ip;
              return inet_ntoa (addr.sin_addr);
         }
protected:
	//接收数据
	virtual void RecData() = 0;
	INetMediator* m_pMediator;//中介者指针，用于将数据传递给中介者

};
