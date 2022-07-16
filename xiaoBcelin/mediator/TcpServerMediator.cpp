#include"TcpServerMediator.h"
#include"TcpServerNet.h"
TcpServerMediator::TcpServerMediator() {
	m_pNet = new TcpServerNet(this);
}
TcpServerMediator::~TcpServerMediator() {
	//回收资源
	if (m_pNet) {
		delete m_pNet;
		m_pNet = nullptr;
	}
}
//开启网络
bool TcpServerMediator::OpenNet() {
	if (!m_pNet->InitNet()) {
		return false;
	}

	return true;
}
//关闭网络
void TcpServerMediator::CloseNet() {
	m_pNet->UninitNet();
}
//发送数据
bool TcpServerMediator::SendData(long lSendlp, char* buf, int nLen) {
	if (!m_pNet->SendData(lSendlp, buf, nLen)) {
		return false;
	}
	return true;
}
//处理数据

void TcpServerMediator::DealData(long lSendlp, char* buf, int nLen) {
	cout <<"Server"<< buf << endl;
	SendData(lSendlp, buf, nLen);
	//TODO:把接收到的数据传给kernrl
}