#include"INet.h"
#include"UdpNet.h"
#include"INetMediator.h"
#include"pack.h"

UdpNet::UdpNet(INetMediator* pMediator):m_sock(INVALID_SOCKET), m_hThreadHendle(0){
	m_pMediator = pMediator;
    m_isStop = false;
}
UdpNet::~UdpNet() {

}//使用时父类指针指向子类对象，需要使用虚析构防止内存泄漏
//初始化网络
 bool UdpNet::InitNet() {
     WORD wVersionRequested;
     WSADATA wsaData;
     int err;
     // 1、选项目——加载库WSAStartup
     wVersionRequested = MAKEWORD(2, 2);

     err = WSAStartup(wVersionRequested, &wsaData);
     if (err != 0) {
         /* Tell the user that we could not find a usable */
         /* Winsock DLL.                                  */
         printf("WSAStartup failed with error: %d\n", err);
         return false;
     }
     //2、雇人——创建套接字socket
     //SOCKET sock = INVALID_SOCKET;
     m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
     if (m_sock == INVALID_SOCKET) {
         cout << "sock:错误" << WSAGetLastError() << endl;
         WSACleanup();
     }
     else {
         cout << "sock:成功" << endl;
     }
     //3、摆摊——绑定IP地址bind
     sockaddr_in addrServer;
     addrServer.sin_family = AF_INET;
     addrServer.sin_port = htons( _DEF_UDP_PORT);//将整型变量从主机字节序转换成网络字节序
     addrServer.sin_addr.S_un.S_addr = INADDR_ANY;//服务器需要绑定任意网卡
     err = bind(m_sock,(sockaddr*) & addrServer,sizeof(addrServer));
     if (SOCKET_ERROR == err) {
         cout << "bind error : " << WSAGetLastError() << endl; //TOD0:关闭套接字
         return false;
     }
     else {
         cout << "bind success." << endl;
     }
    //4、申请广播权限
     BOOL bval = true;
     setsockopt(m_sock,SOL_SOCKET,SO_BROADCAST,(char*) &bval,sizeof(bval));
      // TOD0:5、创建接收线程
         // CreateThread是WIN APl，退出使用的是ExitThread。
         //如果在线程中使用了strcpy这类C运行时库函数，会创建内存块，但是在ExitThread退出线程时，不会回收创建的内存块，会造成内存泄漏。ll _beginthreadex，退出使用的是_endthreadex，会先回收内存空间，再调用ExitThread退出线程。
     m_hThreadHendle = (HANDLE)_beginthreadex(NULL,0, &RecvThread, this,0,0);

	 return true;
}
 unsigned int _stdcall UdpNet:: RecvThread(void* lpvoid){
     UdpNet* pThis = (UdpNet*)lpvoid;
     
     pThis->RecData();
     cout << "ss" << endl;
     return 1;
}
//关闭网络,关闭套接字、卸载库
 void UdpNet::UninitNet() {
     //TODO：关闭线程
     if (m_hThreadHendle) {
         // 让线程自己结束循环
         //m_isStop = true;
         //等待一会，查看线程是否已经退出
             if (WAIT_TIMEOUT == WaitForSingleObject(m_hThreadHendle,100)) {
                 //如果线程没有自己结束，就杀死线程
                 TerminateThread(m_hThreadHendle, - 1);
             }
         //关闭句柄
         CloseHandle(m_hThreadHendle); m_hThreadHendle = NULL;
     }
     //关闭套接字
     if (m_sock && m_sock != INVALID_SOCKET) {
         closesocket(m_sock);
     }
}
//发送数据
 bool UdpNet::SendData(long iSendIp, char* buf, int nlen) {
     sockaddr_in addrServer;
     addrServer.sin_family = AF_INET;
     addrServer.sin_port = htons(_DEF_UDP_PORT);//将整型变量从主机字节序转换成网络字节序
     addrServer.sin_addr.S_un.S_addr = iSendIp;//绑定要发送的ip
     if (sendto(m_sock, buf, nlen, 0, (sockaddr*)&addrServer, sizeof(addrServer)) <= 0) {
         cout << "sendto eror:" << WSAGetLastError() << endl;
         return false;
     }
     return true;
}

	//接收数据
 void UdpNet::RecData() {
     char recvBuf[_DEF_UDP_PORT_zijie] = "";
     int nRecvNum = 0;
     sockaddr_in addrClient;
     int addrClientsize = sizeof(addrClient);
     while (!m_isStop) {
         nRecvNum = recvfrom(m_sock, recvBuf, sizeof(recvBuf), 0, (sockaddr*)&addrClient, &addrClientsize);
         if (nRecvNum > 0) {
             // 接收到数据
                 char* packBuf = new char[nRecvNum];
                 memcpy(packBuf,recvBuf,nRecvNum);
             if (m_pMediator) {
                   //把数据传递给中介者类
                 m_pMediator->DealData(addrClient.sin_addr.S_un.S_addr,packBuf,nRecvNum);
                  // TODO:回收packBuf，在处理完数据以后
             }
         }
     }
}