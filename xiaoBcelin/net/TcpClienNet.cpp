#include"INet.h"
#include"TcpClienNet.h"
#include"TcpClienMediator.h"

#include"pack.h"
//#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

TcpClienNet::TcpClienNet(INetMediator* pMediator) :m_isLoadlib(false),m_sock(INVALID_SOCKET), m_hThreadHendle(0) {
    m_pMediator = pMediator;
    m_isStop = false;
}
TcpClienNet::~TcpClienNet() {

}//使用时父类指针指向子类对象，需要使用虚析构防止内存泄漏
//初始化网络，加载库、创建套接字、连接服务器、创建一个接收线程
bool TcpClienNet::InitNet() {
    if( !m_isLoadlib )
    {
        //1.做个小买卖 炸串 --加载库
        WORD wVersionRequested;
        WSADATA wsaData;
        int err;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
        wVersionRequested = MAKEWORD(2, 2);

        err = WSAStartup(wVersionRequested, &wsaData);
        if (err != 0) {
            return false;
        }

        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
            WSACleanup();
            return false;
        }
        m_isLoadlib = true;
    }
    //2、雇人——创建套接字socket
    //SOCKET sock = INVALID_SOCKET;
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sock == INVALID_SOCKET) {
        std::cout << "sock:错误" << WSAGetLastError() <<  std::endl;
        WSACleanup();
    }
    else {
         std::cout << "sock:成功" <<  std::endl;
    }
    int err;
    //3、连接服务器
    sockaddr_in addr;
    addr.sin_family = AF_INET ;
    addr.sin_addr.S_un.S_addr = inet_addr( _DEF_TCP_SERVER_IP );  /*inet_addr("192.168.31.115")*/ ;  //绑定任意网卡
    addr.sin_port = htons( _DEF_TCP_PORT );  //htons 转换为网络字节序 大端存储  43232


    if( connect( m_sock ,(const sockaddr* ) &addr , sizeof(addr) ) == SOCKET_ERROR )
    {
        //UnInitNet();
        std::cout << "12bind error : " << WSAGetLastError() <<  std::endl;
        return false;
    }
    //设置客户端发送缓冲区
    int nSendBuf=128*1024;//设置为64K
    setsockopt(m_sock,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));
    //设置客户端接收缓冲区
    int nRecvBuf=256*1024;//设置为64K
    setsockopt(m_sock,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
    //禁用 TCP-NODELAY
    int value = 1;
    setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (char *)&value, sizeof(int));
    ////4、申请广播权限
    //BOOL bval = true;
    //setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST, (char*)&bval, sizeof(bval));
    // TOD0:5、创建接收线程
       // CreateThread是WIN APl，退出使用的是ExitThread。
       //如果在线程中使用了strcpy这类C运行时库函数，会创建内存块，但是在ExitThread退出线程时，不会回收创建的内存块，会造成内存泄漏。ll _beginthreadex，退出使用的是_endthreadex，会先回收内存空间，再调用ExitThread退出线程。
    m_hThreadHendle = (HANDLE)_beginthreadex(NULL, 0, &RecvThread, this, 0, 0);

    return true;
}
unsigned int _stdcall TcpClienNet::RecvThread(void* lpvoid) {
    TcpClienNet* pThis = (TcpClienNet*)lpvoid;

    pThis->RecData();
     std::cout << "ss" <<  std::endl;
    return 1;
}
//关闭网络,关闭套接字、卸载库
void TcpClienNet::UninitNet() {
    //TODO：关闭线程//// 为什么先关闭线程在关闭句柄，因为每个线程创建时操作系统给每个线程分配内核对象，内核对象有个引用计数为二，关闭线程计数减一
    //关闭句柄计数减一，回收内核对象需要让引用计数变为零
    if (m_hThreadHendle) {
        // 让线程自己结束循环
        m_isStop = true;
        //强制杀死线程会使公共资源出错
        //等待一会，查看线程是否已经退出
        if (WAIT_TIMEOUT == WaitForSingleObject(m_hThreadHendle, 100)) {
            //如果线程没有自己结束，就杀死线程
            TerminateThread(m_hThreadHendle, -1);
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
bool TcpClienNet::SendData(long iSendIp, char* szbuf, int nlen) {


     std::cout << "TcpClienNet::SendData" <<  std::endl;
    //1、校验参数
    if (!szbuf || nlen <= 0) {
         std::cout << "parameter eror" <<  std::endl;
        return false;
    }
       iSendIp = m_sock;
       int DataLen = nlen + 4;
       std::vector<char> vecbuf;
       vecbuf.resize( DataLen );

    //   char* buf = new char[ DataLen ];
       char* buf = &*vecbuf.begin();
       char* tmp = buf;
       *(int*) tmp = nlen;
       tmp+= sizeof(int);

       memcpy( tmp , szbuf , nlen);

    //3、再发包内容
       int err;
    if (err=send( iSendIp ,buf,DataLen , 0)){
         std::cout <<err<< "sendto eror:" << WSAGetLastError() <<  std::endl;
        return false;
    }

    return true;
}

//接收数据
void TcpClienNet::RecData() {
    
    int nPackSize = 0; // 存储包大小
        int nRes = 0;
        while( !m_isStop )
        {
            //接收先接收包大小 在接受数据包
            nRes = recv( m_sock , (char*)&nPackSize , sizeof(int) , 0   );
            //从接收缓冲区拷贝包大小
            if( nRes == 0)
            {
                //m_isConnected = false;
                //m_pMediator->disConnect();
                closesocket(m_sock);
                m_sock = INVALID_SOCKET;
                break;
            }
            if( nRes < 0 )
            {
                if((errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN))
                {
                    continue;
                }
                /*
                EAGAIN：套接字已标记为非阻塞，而接收操作被阻塞或者接收超时
                EBADF：sock不是有效的描述词
                ECONNREFUSE：远程主机阻绝网络连接
                EWOULDBLOCK：用于非阻塞模式，不需要重新读或者写
                EFAULT：内存空间访问出错
                EINTR：操作被信号中断
                EINVAL：参数无效
                ENOMEM：内存不足
                ENOTCONN：与面向连接关联的套接字尚未被连接上
                ENOTSOCK：sock索引的不是套接字 当返回值是0时，为正常关闭连接
                */
                closesocket(m_sock);
                m_sock = INVALID_SOCKET;
                break;
            }
            int offset = 0;
            char * buf = new char[nPackSize];
            while( nPackSize )
            {
                nRes = recv( m_sock , buf + offset , nPackSize  , 0   );
                if( nRes > 0 )
                {
                    nPackSize -= nRes;
                    offset += nRes;
                }
            }
            if (m_pMediator) {
                //把数据传递给中介者类
                m_pMediator->DealData(m_sock, buf, offset);
                // TODO:回收packBuf，在处理完数据以后
            }
        }

}
