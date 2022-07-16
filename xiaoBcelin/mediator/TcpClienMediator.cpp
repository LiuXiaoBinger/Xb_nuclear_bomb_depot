#include"TcpClienMediator.h"
#include"TcpClienNet.h"
TcpClientMediator::TcpClientMediator() {
	m_pNet = new TcpClienNet(this);
}
TcpClientMediator::~TcpClientMediator() {
	//回收资源
	if (m_pNet) {
		delete m_pNet;
		m_pNet = nullptr;
	}
}
//开启网络
bool TcpClientMediator::OpenNet() {
	if (!m_pNet->InitNet()) {
		return false;
	}

	return true;
}
//关闭网络
void TcpClientMediator::CloseNet() {
	m_pNet->UninitNet();
}
//发送数据
bool TcpClientMediator::SendData(long lSendlp, char* buf, int nLen) {
	if (!m_pNet->SendData(lSendlp, buf, nLen)) {
         std::cout << "TcpClientMediator::SendData" <<  std::endl;
		return false;
	}
	return true;
}
//处理数据

void TcpClientMediator::DealData(long lSendlp, char* buf, int nLen) {
    std::cout<<__func__<<std::endl;
     //std::cout << "Clien" << buf <<  std::endl;
	//TODO:把接收到的数据传给kernrl
     Q_EMIT SIG_ReadyData(lSendlp, buf, nLen);
}
