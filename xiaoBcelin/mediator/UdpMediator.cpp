#include"UdpMediator.h"
#include"UdpNet.h"
UdpMediator::UdpMediator() {
	m_pNet=new UdpNet(this);
}
UdpMediator::~UdpMediator() {
	//回收资源
	if (m_pNet) {
		delete m_pNet;
		m_pNet = nullptr;
	}
}
//开启网络
bool UdpMediator::OpenNet() {
	if (!m_pNet->InitNet()) {
		return false;
	}

	return true;
}
//关闭网络
void UdpMediator::CloseNet() {
	m_pNet->UninitNet();
}
//发送数据
bool UdpMediator::SendData(long lSendlp, char* buf, int nLen) {
	if (!m_pNet->SendData(lSendlp, buf, nLen)) {
		return false;
	}
	return true;
}
//处理数据

void UdpMediator::DealData(long lSendlp, char* buf, int nLen) {
	cout << buf << endl;
	//TODO:把接收到的数据传给kernrl
}