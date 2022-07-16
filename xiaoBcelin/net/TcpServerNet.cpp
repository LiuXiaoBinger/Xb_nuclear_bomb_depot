
#include"TcpServerNet.h"
#include"TcpServerMediator.h"
#include"pack.h"

TcpServerNet::TcpServerNet(INetMediator* pMediator) :m_sock(INVALID_SOCKET), m_hThreadHendle(0){
    m_pMediator = pMediator;
    m_isStop = false;
}
TcpServerNet::~TcpServerNet() {

}//使用时父类指针指向子类对象，需要使用虚析构防止内存泄漏
//初始化网络
bool TcpServerNet::InitNet() {
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
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
    addrServer.sin_port = htons(_DEF_TCP_PORT);//将整型变量从主机字节序转换成网络字节序
    addrServer.sin_addr.S_un.S_addr = INADDR_ANY;//服务器需要绑定任意网卡
    err = bind(m_sock, (sockaddr*)&addrServer, sizeof(addrServer));
    if (SOCKET_ERROR == err) {
        cout << "bind error : " << WSAGetLastError() << endl; //TOD0:关闭套接字
        return false;
    }
    else {
        cout << "bind success." << endl;
    }
    //4.监听
    err = listen(m_sock, 10);
    if (SOCKET_ERROR == err) {
        cout << "listen error : " << WSAGetLastError() << endl; //TOD0:关闭套接字
        return false;
    }
    else {
        cout << "listen success." << endl;
    }
    // TOD0:5、创建接收连接线程
       // CreateThread是WIN APl，退出使用的是ExitThread。
       //如果在线程中使用了strcpy这类C运行时库函数，会创建内存块，但是在ExitThread退出线程时，不会回收创建的内存块，会造成内存泄漏。ll _beginthreadex，退出使用的是_endthreadex，会先回收内存空间，再调用ExitThread退出线程。
    m_hThreadHendle = (HANDLE)_beginthreadex(NULL, 0, &AcceptThread, this, 0, 0);
    if (m_hThreadHendle) {
        m_hThreaHeadleList.push_back(m_hThreadHendle);
    }
    return true;
}
unsigned int _stdcall TcpServerNet::AcceptThread(void* lpvoid) {
      TcpServerNet* pThis = (TcpServerNet*)lpvoid;
      sockaddr_in addrClinent;
      int addrClinentsize = sizeof(addrClinent);
      while (!pThis->m_isStop) {
          SOCKET sock = accept(pThis->m_sock, (sockaddr*)&addrClinent, &addrClinentsize);
              cout << "ip:" << inet_ntoa(addrClinent.sin_addr) << endl;
              //给链接成功的客户端创建一个接收数据的线程
              unsigned int nThreadid = 0;
              HANDLE hThreadHendle= (HANDLE)_beginthreadex(NULL, 0, &RecvThread, pThis, 0, &nThreadid);
              //保存句柄
              pThis->m_hThreaHeadleList.push_back(hThreadHendle);
              //保存socket和对应线程id
              pThis->m_mapThreadldToSocket[nThreadid] = sock;
      }
     
    return 1;
}
unsigned int _stdcall TcpServerNet::RecvThread(void* lpvoid) {
      TcpServerNet* pThis = (TcpServerNet*)lpvoid;

      pThis->RecData();
      cout << "ss" << endl;
    return 1;
}
//关闭网络,退出线程,关闭套接字、卸载库
void TcpServerNet::UninitNet() {
    //TODO：关闭线程
     // 让线程自己结束循环
        m_isStop = true;
        for (auto ite = m_hThreaHeadleList.begin(); ite != m_hThreaHeadleList.end();) {
            if (*ite) {

                //等待一会，查看线程是否已经退出
                if (WAIT_TIMEOUT == WaitForSingleObject(*ite, 100)) {
                    //如果线程没有自己结束，就杀死线程
                    TerminateThread(*ite, -1);
                }
                //关闭句柄
                CloseHandle(*ite);
                *ite = NULL;
            }
            ite = m_hThreaHeadleList.erase(ite);
          
        }
        //关闭套接字
        if (m_sock && m_sock != INVALID_SOCKET) {
            closesocket(m_sock);
        }
        for (auto ite = m_mapThreadldToSocket.begin(); ite != m_mapThreadldToSocket.end();) {
            if (ite->second && ite->second != INVALID_SOCKET) {
                closesocket(ite->second);

            }
            ite = m_mapThreadldToSocket.erase(ite);
        }
    //卸载库
        WSACleanup();
}
//发送数据
bool TcpServerNet::SendData(long iSendIp, char* buf, int nlen) {
    //1、校验参数
    if (!buf || nlen <= 0) {
        cout << "Serveparameter eror" << endl;
        return false;
    }
    //2、先发包大小
    if (send(iSendIp, (char*)&nlen, sizeof(int), 0) <= 0) {
        cout << "Servesendto eror:" << WSAGetLastError() << endl;
        return false;
    }
    //3、再发包内容
    if (send(iSendIp, buf, nlen, 0) <= 0) {
        cout << "Servesendto eror:" << WSAGetLastError() << endl;
        return false;
    }
    return true;
}

//接收数据
void TcpServerNet::RecData() {
    // 1、获取与客户端通信的socket
    //因为接收数据线程是创建即运行，但是把线程id和对应的套接字存入map中还需要一点时间，所以需要休眠一会
    Sleep(100) ;
    SOCKET sock = m_mapThreadldToSocket[GetCurrentThreadId()];
    // 2、判断socket的合法性
    if (!sock || sock == INVALID_SOCKET) {
        return;
    }
    //3.接收数据
    int nRecvNum = 0;
    int packsize = 0;
    int offset = 0;

    while (!m_isStop) {
        //先接受包大小
        nRecvNum = recv(sock, (char*)&packsize, sizeof(int), 0);
        if (nRecvNum > 0) {
            // 接收到数据
            char* packBuf = new char[packsize];
            while (packsize) {
                nRecvNum = recv(sock, packBuf+offset, packsize, 0);
                if (nRecvNum > 0) {
                    offset += nRecvNum;
                    packsize -= nRecvNum;

                }
            }
            if (m_pMediator) {
                //把数据传递给中介者类
                m_pMediator->DealData(sock, packBuf, offset);
                // TODO:回收packBuf，在处理完数据以后
            }
        }
    }

}