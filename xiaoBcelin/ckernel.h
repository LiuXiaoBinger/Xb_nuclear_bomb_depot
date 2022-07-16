#ifndef CKERNEL_H
#define CKERNEL_H
#include<QObject>
#include"userlten.h"
#include "chatdialog.h"
#include"mychatdialog.h"
#include"INetMediator.h"
#include"TcpClienMediator.h"
#include"pack.h"
#include"logindialog.h"
#include"userlten.h"
#include<QMap>
#include<iostream>
#include"videoread.h"
#include<QBuffer>
#include "threadworker.h"
#include"screenread.h"
using namespace std;
//定义函数指针
class Ckernel;
typedef void (Ckernel::*PFUN)(long iSendIp, char* buf, int nlen);
//声明视频发送工作者
class SendVideoWorker;
class Ckernel : public QObject
{
    Q_OBJECT
public:
    explicit Ckernel(QObject *parent = nullptr);
        ~Ckernel();
    //将kernel设置成单列者模式
    static Ckernel* GetInstance();

    //utf8(QString)->gb2312(char*)
    static void utf8ToGb312(char* gbBuf,int nLen ,QString utf8);
    //gb2312->utf8
    static QString gb2312Toutf8(char*gbBuf);
    //初始化协议映射表
    void setProtocalMp();
    //处理注册回复
    void dealRedisteRs(long iSendIp, char* buf, int nlen);
    //处理登录回复
    void dealLonginRs(long iSendIp, char* buf, int nlen);
    //处理好友信息请求
    void delFriendInfoRq(long iSendIp, char* buf, int nlen);
    //处理聊天请求
    void dealChatRq (long iSendIp, char* buf, int nlen) ;
    //处理聊天回复
    void dealChatRs (long iSendIp, char* buf, int nlen) ;
    //处理添加好友请求
    void dealaddFriendRq (long iSendIp, char* buf, int nlen) ;
    //处理添加好友回复
    void dealaddFriendRs (long iSendIp, char* buf, int nlen) ;
    //处理下线请求
    void dealofflineRq(long iSendIp, char* buf, int nlen) ;
    //处理语音通话请求
    void deal_voicechatRp(long lsendIp, char *buf, int nLen);
    //处理语音通话回复
    void deal_voicechatRs(long lsendIp, char *buf, int nLen);
    //处理视频聊天请求
    void slot_dealVideRq(long lsendIp, char *buf, int nLen);
     //处理视频聊天回复
    void slot_dealVideRs(long lsendIp, char *buf, int nLen);

signals:
    //发送给视频工作者（线程）信号
     void SIG_SendVideo( char* buf , int nlen);
    //槽函数
public slots:
    //发送视频聊天请求
    void slot_viderq (int id);
    //发送语音tong聊天请求
    void slot_voice ( int id);
    //发送音频帧
    void slot_audioFrame(int id,QByteArray ba);
    //给对端发送关闭语音通话通知
    void solt_deal_audio_close(int id);
    //接收视频模块发送图片帧
   void solt_sendVideoFrame( QImage img);

public slots:
    //处理接收到的所有数据
    void slot_ReadyData(long iSendIp, char* buf, int nlen);
    //处理注册请求信号
    void slot_RegisterCommit(QString name,QString tel,QString password);
    //处理登录请求
    void slot_LoginCommit(QString tel,QString password);
    //处理聊天信息
    void slot_sendChatMsg(int id,QString content);
    //处理用户点击好友头像的槽函数
    void slot_userItemClicked(int id);
    //处理添加好友的信号
    void slot_addFriend();

    //处理关闭登录和注册页面的信号
    void slot_close ();
    //处理关闭好友列表页面的信号
    void slot_offline () ;
    //处理对端发送关闭语音通话通知
    void deal_audio_close(long lsendIp, char *buf, int nlen);
    //音频帧处理
    void slot_dealAudioFrameRq(long lsendIp, char *buf, int nlen);
    //视频帧处理
    void slot_dealVideoFrameRq(long lsendIp, char *buf, int nlen);
    //多线程发送视频
    void slot_SendVideo( char* buf , int nlen);

public:
    QString m_name;
private:
    int m_id;

    mychatDialog* m_mainWnd;
    INetMediator*m_tcpClient;
    loginDialog*m_loginDia;
    //协议映射表数据，协议头->函数地址
    PFUN m_netProtocalMap[_DEF_PROTOCAL_COUNT];
    //保存id和userItem的映射关系
    QMap<int ,Userlten*>m_mapIdToUserItem;
    QMap<int ,chatdialog*>m_mapIdToChatdlg;

    //音频
    AudioRead *auRead;
    AudioWrite *auWrite;
    //视频模块
    VideoRead *videorw;
    ScreenRead*m_pScreenRead;//桌面
    QSharedPointer<SendVideoWorker> m_pSendVideoWorker;

    enum client_type{ audio_client = 0 , video_client };
    INetMediator * m_pAVClient[2];

};
class SendVideoWorker : public ThreadWorker
{
    Q_OBJECT
public slots:
    void slot_sendVideo( char * buf , int nlen ){
        Ckernel::GetInstance()->slot_SendVideo( buf , nlen );
    }
};
#endif // CKERNEL_H
