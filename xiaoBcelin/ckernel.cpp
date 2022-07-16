#include "ckernel.h"
#include"TcpClienMediator.h"
#include<QMessageBox>
#include<QDebug>
#include<QInputDialog>
#include<QTextCodec> //QT += core5compat
#include"INet.h"
#include<QTime>
//计算数组下标的宏替换
#define NetPackFunMap(a) m_netProtocalMap[a- _DEF_PROTOCAL_BASE-1]
 //将kernel设置成单列者模式
Ckernel *Ckernel::GetInstance()
{
    static Ckernel kernel;
    return &kernel;
}
//utf8(QString)->gb2312(char*)
 void Ckernel::utf8ToGb312(char* gbBuf,int nLen ,QString utf8){
    QTextCodec*gb2312=QTextCodec::codecForName("gb2312");
    QByteArray ba=gb2312->fromUnicode(utf8);
    strcpy_s(gbBuf,nLen,ba.data());
}
//gb2312->utf8
 QString Ckernel::gb2312Toutf8(char*gbBuf){
     QTextCodec* gb2312 = QTextCodec :: codecForName ( "gb2312");
     return gb2312->toUnicode (gbBuf);

}
Ckernel::Ckernel(QObject *parent)
    : QObject{parent},m_id(0)
{
       setProtocalMp();
        //定义主窗口对话框，并且提示
       m_mainWnd=new mychatDialog;
       //m_mainWnd->show();
       //创建添加好友的信号和槽函数
       connect(m_mainWnd,&mychatDialog::SIG_addFriend,this,&Ckernel::slot_addFriend);
       //绑定关闭好友列表页面的信号和槽函数
       connect (m_mainWnd,SIGNAL(SIG_close ()),
       this,SLOT(slot_offline ()));
       //音频读写
       auRead=new AudioRead;
       auWrite=new AudioWrite;
       //auRead读音频帧转到kenrnel
       connect(auRead,&AudioRead::SIG_audioFrame,this,&Ckernel::slot_audioFrame);
       //视频读与播放
       videorw=new VideoRead;
       //videorw视频频帧转到kenrnel
       connect(videorw,&VideoRead::SIG_sendVideoFrame,this,&Ckernel::solt_sendVideoFrame);

       //创建登录界面
       m_loginDia=new loginDialog;
       m_loginDia->show();
       connect(m_loginDia,&loginDialog::SIG_RegisterCommit,this,&Ckernel::slot_RegisterCommit);
       connect(m_loginDia,&loginDialog::SIG_LoginCommit,this,&Ckernel::slot_LoginCommit);

       //发送视频工作者
       m_pSendVideoWorker= QSharedPointer<SendVideoWorker>( new SendVideoWorker);
       connect( this , SIGNAL(SIG_SendVideo(char*,int)) ,
                m_pSendVideoWorker.data() , SLOT( slot_sendVideo(char*,int)) );

       //绑定处理关闭登录和注册页面的信号和槽函数
       connect (m_loginDia, SIGNAL(SIG_close() ) ,
       this, SLOT(slot_close ()));
       //采集桌面
       m_pScreenRead=new  ScreenRead;
       connect(m_pScreenRead,&ScreenRead::SIG_getScreenFrame,this,&Ckernel::solt_sendVideoFrame);
       //初始化中介者类对象
       m_tcpClient=new TcpClientMediator;
       //绑定发送所有接收到的数据的信号和槽函数
       connect (m_tcpClient,SIGNAL (SIG_ReadyData (long, char*,int)) ,this, SLOT(slot_ReadyData (long , char* ,int) ));
       //打开网络
       if ( !m_tcpClient->OpenNet() ){
            QMessageBox ::about(m_mainWnd,"提示","打开网络失败");
            exit (0);     
        }
       //音频和视频的网络连接
       for( int i = 0 ; i < 2 ; i++){
           m_pAVClient[i] = new TcpClientMediator;
           m_pAVClient[i]->OpenNet();
           connect( m_pAVClient[i] , SIGNAL( SIG_ReadyData(long long,char*,int))
                    , this , SLOT( slot_dealData(long long,char*,int) ) );
       }
}
Ckernel::~Ckernel(){
    //回收资源
    //回收主窗口资源
    if (m_mainWnd){
        //隐藏窗口
        m_mainWnd->hide();
        delete m_mainWnd;
        m_mainWnd = NULL;
    }
    // 2、回收中介者类对象
    if (m_tcpClient){
        //关闭网络
        m_tcpClient->CloseNet () ;
        delete m_tcpClient;
        m_tcpClient = NULL;
    }
    exit (0);

}


//初始化协议映射表
void Ckernel::setProtocalMp()
{
    //初始化
    memset(m_netProtocalMap,0,_DEF_PROTOCAL_COUNT);
    //绑定协议头和处理函数
    NetPackFunMap(_DEF_PACK_TCP_REGISTER_RS)=&Ckernel::dealRedisteRs;
    NetPackFunMap(_DEF_PACK_TCP_LOGIN_RS)=&Ckernel::dealLonginRs;
    NetPackFunMap(_DEF_PACK_TCP_FRIEND_INFO)=&Ckernel::delFriendInfoRq;
    NetPackFunMap(_DEF_PACK_TCP_CHAT_RS)=&Ckernel::dealChatRs;
    NetPackFunMap(_DEF_PACK_TCP_CHAT_RQ)=&Ckernel::dealChatRq;
    NetPackFunMap(_DEF_TCP_voice_RQ)=&Ckernel::deal_voicechatRp;
    NetPackFunMap(_DEF_TCP_voice_RS)=&Ckernel::deal_voicechatRs;
    NetPackFunMap(_DEF_PACK_TCP_ADDFRIEND_RQ)=&Ckernel::dealaddFriendRq;
    NetPackFunMap(_DEF_PACK_TCP_ADDFRIEND_RS)=&Ckernel::dealaddFriendRs;
    NetPackFunMap(_DEF_PACK_TCP_OFFLINE_RQ)=&Ckernel::dealofflineRq;

     NetPackFunMap(EXIT_VIDEO_CALL_RQ)=&Ckernel::deal_audio_close;
      NetPackFunMap(_DEF_TCP_AUDIO_CHAT)=&Ckernel::slot_dealAudioFrameRq;
       NetPackFunMap(_DEF_TCP_VIDEO_RS)=&Ckernel::slot_dealVideRs;
        NetPackFunMap(_DEF_TCP_VIDEO_RQ)=&Ckernel::slot_dealVideRq;
         NetPackFunMap(_DEF_TCP_VIDEO_CHAT)=&Ckernel::slot_dealVideoFrameRq;
}
//处理注册回复
void Ckernel::dealRedisteRs(long iSendIp, char *buf, int nlen)
{
     qDebug()<<__func__;
     //拆包
     STRU_TGP_REGISTER_RS* rs = (STRU_TGP_REGISTER_RS* ) buf;//根据注册结果显示提示信息
     switch (rs->result){
         case parameter_error:
                QMessageBox::about(m_loginDia,"提示","注册失败，输入信息有误");break;
         case user_is_exist :
                QMessageBox :: about (m_loginDia,"提示","注册失败，用户己存在");break;
         case register_success:
                QMessageBox ::about (m_loginDia,"提示","注册成功");break;
         default:
                break;
     }

}
//处理登录回复
void Ckernel::dealLonginRs(long iSendIp, char *buf, int nlen)
{
    qDebug()<<__func__;
    //拆包
    STRU_TCP_LOGIN_RS* rs = (STRU_TCP_LOGIN_RS* ) buf;
     //注册 视频和音频的fd
    STRU_AUDIO_REGISTER rq_audio;
    rq_audio.m_userid = m_id;

    STRU_VIDEO_REGISTER rq_video;
    rq_video.m_userid = m_id;
    //根据注册结果显示提示信息
    switch (rs->result){
        case parameter_error:
               QMessageBox::about(m_loginDia,"提示","登录失败，输入信息有误");break;
        case user_not_exist :
               QMessageBox :: about (m_loginDia,"提示","登录失败，用户不存在");break;
        case password_error :
           QMessageBox :: about (m_loginDia,"提示","登录失败，密码错误");break;
        case register_success:
               //QMessageBox ::about (m_loginDia,"提示","登录成功");break;
               //1.隐藏登录界面
               m_loginDia->hide();
               //2、显示好友列表
               m_mainWnd->showNormal();
               //保存用户id
               m_id=rs->userld;




               m_pAVClient[audio_client]->
                       SendData(0,(char*)&rq_audio,sizeof(rq_audio));
               m_pAVClient[video_client]->
                       SendData(0,(char*)&rq_video,sizeof(rq_video));
        break;
        default:
               break;
    }
}
//处理好友信息请求
void Ckernel::delFriendInfoRq(long iSendIp, char *buf, int nlen)
{
    //1、拆包
    STRU_TCP_FRIEND_INFO* info = (STRU_TCP_FRIEND_INFO* ) buf;
    // 1.1、中文编码格式转换
    QString nameTemp =gb2312Toutf8 (info->name) ;
    QString feelingTemp = gb2312Toutf8(info->feeling) ;
    //2、查看是不是自己的信息
    if (info->userld == m_id){
        //2.1、更新用户界面
        m_mainWnd->setInfo (nameTemp,feelingTemp,info->iconld);
        //2.2保存自己名字
        m_name=nameTemp;

        //2.3直接返回
        return;
    }
    if(m_mapIdToUserItem.find(info->userld)==m_mapIdToUserItem.end()){
        //3、窗口之前没有这个好友
        //3.1、new一个useritem
        Userlten*item=new Userlten;
        //3.2、设置Userlten属性
        item->setInfo(nameTemp,feelingTemp,info->state,info->iconld,info->userld);
        //3.3、把Userlten添加到列表上
        m_mainWnd->addFriend(item);
        //3.4、绑定点击用户头像的信号和槽函数
        connect(item,&Userlten::SIG_UserItemClicked,this,&Ckernel::slot_userItemClicked);
        //3.5保存new出来的Userlten
        m_mapIdToUserItem[info->userld]=item;
        //3.6、new聊天窗口
        chatdialog *chat=new chatdialog;
        //3.7、设置聊天窗口信息
        chat->setInfo(nameTemp,info->userld);
        //3.8、绑定发送聊天内容的信号和槽函数
        connect(chat,&chatdialog::SIG_sendChatMsg,this,&Ckernel::slot_sendChatMsg);
        //管理语音通话暂停开始 结束;
        connect(chat->audQDialog,SIGNAL(SIG_START(int)),auRead,SLOT(start(int)));
        connect(chat->audQDialog,SIGNAL(SIG_STOP()),auRead,SLOT(pause()));
        connect(chat->audQDialog,SIGNAL(SIG_close(int)),auRead,SLOT(solt_deal_close(int)));
        //关闭音频通话通知对端
        connect(chat->audQDialog,SIGNAL(SIG_close(int)),this,SLOT(solt_deal_audio_close(int)));
        //-----------------------------------------------------------
        //绑定视频通话请求信号和槽
        connect(chat,SIGNAL(SIG_vide(int)),this,SLOT(slot_viderq(int)));
        //管理视频通话暂停开始 结束;
        connect(chat->videqDialog,SIGNAL(SIG_START(int)),videorw,SLOT(slot_openVideo(int)));
        connect(chat->videqDialog,SIGNAL(SIG_STOP()),videorw,SLOT(slot_closeVideo()));
        connect(chat->videqDialog,SIGNAL(SIG_screenStart()),m_pScreenRead,SLOT(slot_openVideo()));
        connect(chat->videqDialog,SIGNAL(SIG_screenPause()),m_pScreenRead,SLOT(slot_closeVideo()));

        connect(chat->videqDialog,SIGNAL(SIG_close(int)),m_pScreenRead,SLOT(slot_closeVideo()));
        connect(chat->videqDialog,SIGNAL(SIG_close(int)),this,SLOT(solt_deal_audio_close(int)));
        connect(chat->videqDialog,SIGNAL(SIG_close(int)),videorw,SLOT(solt_deal_close(int)));
        //采集到自己的数据加载到窗口中
        connect(videorw,SIGNAL(SIG_sendVideoFrame(QImage)),chat->videqDialog,SLOT(slot_setmyimg(QImage)));
        connect(m_pScreenRead,SIGNAL(SIG_getScreenFrame(QImage)),chat->videqDialog,SLOT(slot_setmyimg(QImage)));
        //绑定语音包捕获与kernel转发this
        connect(auRead,SIGNAL(SIG_audioFrame( int id,QByteArray bt )),this,SLOT(slot_audioFrame(int ,QByteArray )));
        //kenel接收到语音包&发送到音频显示模块
        connect(this,SIGNAL(SIG_audioFrame( int id,QByteArray bt )),auWrite,SLOT(slot_audioFrame(int ,QByteArray )));
        //绑定语音通话请求信号和槽
        connect(chat,SIGNAL(SIG_voice (int)),this,SLOT(slot_voice (int)));
        //3.9、聊天窗口保存map中管理
        m_mapIdToChatdlg[info->userld]=chat;

       // connect(videorw,&SIG_sendVideoFrame(int,QImage)),chat->videqDialog,SLOT(slot_setmyimg(int,QImage)));

    }else{
        //4、窗口上已经有这个好友了
        //4.1、取出这个好友的Userlten
        Userlten*item=m_mapIdToUserItem[info->userld];
        //4.2、以前是下线的状态，现在变成上线状态，可以显示好友上线，可以弹窗，也可以打印日志
        if(item&&0==item->m_state&&1==info->state){
            qDebug()<<QString ("您的好友【%1】已上线").arg (nameTemp);

        }
        //4.3、更新好友信息
        item->setInfo(nameTemp,feelingTemp,info->state,info->iconld,info->userld);
    }
}
//处理接收到的所有数据
void Ckernel::slot_ReadyData(long iSendIp, char *buf, int nlen)
{
    qDebug()<<__func__<<":"<<buf;
    //1、去除协议头
    int nType=*(int*)buf;
    //2、根据协议头计算出数组中对应的处理函数
    if(nType>=_DEF_PROTOCAL_BASE+1&&nType<=_DEF_PROTOCAL_BASE+_DEF_PROTOCAL_COUNT){
        //计算出数组下标，取出下标中存的函数指针
        PFUN pf=NetPackFunMap(nType);
        if(pf){
            (this->*pf)( iSendIp,  buf,  nlen);
        }
        delete[]buf;
    }
}
//处理注册请求信号
void Ckernel::slot_RegisterCommit(QString name, QString tel, QString password)
{
        cout<<__func__<<endl;
        //数据格式转换
        std::string nameStr=name.toStdString();
        std::string telstr=tel.toStdString();
        std::string passwordstr=password.toStdString();
        //封包
        STRU_TCP_REGISTER_RQ rp;
        utf8ToGb312(rp.name,sizeof(rp.name),name);
        //strcpy(rp.name,nameStr.c_str());
         strcpy(rp.tel,telstr.c_str());
          strcpy(rp.password,passwordstr.c_str());
        //数据发给服务器
        m_tcpClient->SendData(0,(char*)&rp,sizeof(rp));
}
//处理登录请求信号
void Ckernel::slot_LoginCommit(QString tel, QString password)
{
        cout<<__func__<<endl;
        //数据格式转换

        std::string telstr=tel.toStdString();
        std::string passwordstr=password.toStdString();
        //封包
        STRU_TCP_LOGIN_RQ rp;

         strcpy(rp.tel,telstr.c_str());
         strcpy(rp.password,passwordstr.c_str());
        //数据发给服务器
        m_tcpClient->SendData(0,(char*)&rp,sizeof(rp));
}

//处理聊天信息
void Ckernel::slot_sendChatMsg(int id,QString content){
    qDebug()<<__func__;
    //1、封包
    STRU_TCP_CHAT_RQ rq;
    rq.userld=m_id;
    rq.friendld=id;
    strcpy(rq.content,content.toStdString().c_str());

    //2、把数据发到服务器
    m_tcpClient->SendData(0,(char*)&rq,sizeof(rq));
}
//处理用户点击好友头像的槽函数
void Ckernel::slot_userItemClicked(int id){
    qDebug()<<__func__;
    //查看与好友的聊天窗口是否存在，存在就显示出来
    if (m_mapIdToChatdlg.find(id)!= m_mapIdToChatdlg.end()){
        chatdialog*chatDlg = m_mapIdToChatdlg[id];
        chatDlg->showNormal();
    }
}
//处理添加好友的信号
void Ckernel::slot_addFriend()
{
    qDebug () <<__func__;
    //1.弹出一个添加好友的对话框
    QString strName = QInputDialog::getText (m_mainWnd,"添加好友","请输入好友的名字:");
    //2、校验用户输入数据是否合法
    if (strName.isEmpty()|| strName.length()> 10){
        QMessageBox :: about (m_mainWnd,"提示","输入昵称不合法");
        return;
    }
    //3、判断是不是自己，不能添加自己为好友
    if(m_name == strName){
        QMessageBox :: about (m_mainWnd,"提示","不能添加自己为好友");
        return;
    }
    // 4、判断是不是已经是好友了，如果是好友，提示不能添加
    for (auto ite = m_mapIdToUserItem.begin (); ite != m_mapIdToUserItem.end(); ite ++){
        Userlten* item = *ite;
        if(item->m_name==strName) {
            QMessageBox:: about(m_mainWnd,"提示","已经是好友啦，无需重复添加");
            return;
        }
    }
    // 5、发送添加好友请求给服务端
    STRU_TCP_ADD_FRIEND_RQ rq;
    rq.userld = m_id;
    strcpy(rq.userName, m_name.toStdString().c_str());
    utf8ToGb312(rq.friendName,sizeof(rq.friendName),strName) ;
    m_tcpClient->SendData(0,(char* )&rq, sizeof(rq) );

}

//处理聊天请求
void Ckernel::dealChatRq (long iSendIp, char* buf, int nlen) {
    qDebug()<<__func__;
    //1、拆包
    STRU_TCP_CHAT_RQ* rq=(STRU_TCP_CHAT_RQ*)buf;
    //2、查看聊天窗口是否存在
    if(m_mapIdToChatdlg.find(rq->userld)!=m_mapIdToChatdlg.end()){
        chatdialog*chatDlg = m_mapIdToChatdlg[rq->userld];
        chatDlg->setChatContent(rq->content);
        chatDlg->showNormal();
    }
}
//处理聊天回复
void Ckernel::dealChatRs (long iSendIp, char* buf, int nlen) {
    qDebug()<<__func__;
    //1、拆包
    STRU_TCP_CHAT_RS* rs=(STRU_TCP_CHAT_RS*)buf;
    //2、如果用户在线
    if(m_mapIdToChatdlg.find(rs->friendld)!=m_mapIdToChatdlg.end()){
        chatdialog*chatDlg = m_mapIdToChatdlg[rs->friendld];
        chatDlg->setUserOffline();
        chatDlg->showNormal();
    }
}
//处理添加好友请求
void Ckernel::dealaddFriendRq (long iSendIp, char* buf, int nlen) {
    qDebug()<<__func__;
    //1、拆包
    STRU_TCP_ADD_FRIEND_RQ*rq=(STRU_TCP_ADD_FRIEND_RQ*)buf;
    STRU_TCP_ADD_FRIEND_RS rs;

    //2、弹出提示框，询问是否添加好友
    QString str =QString("用户[%1]请求添加好友，是否同意？").arg(rq->userName);
    if(QMessageBox::Yes==QMessageBox::question(m_mainWnd,"添加好友",str)){
        rs.result=add_success;
    }else{
        rs.result=user_refuse;
    }
    rs.friendld=m_id;
    rs.userld=rq->userld;
    strcpy(rs.friendName,m_name.toStdString().c_str());
    m_tcpClient->SendData(0,(char* )&rs, sizeof(rs) );

}
//处理添加好友回复
void Ckernel::dealaddFriendRs (long iSendIp, char* buf, int nlen) {
    qDebug()<<__func__;
    //1、拆包x
    STRU_TCP_ADD_FRIEND_RS*rs=(STRU_TCP_ADD_FRIEND_RS*)buf;
    //1.1、用户名转码
    QString strname=rs->friendName;gb2312Toutf8(rs->friendName);
    //2、根据结果提示信息
    switch (rs->result) {
    case no_this_user:
        QMessageBox::about(m_mainWnd,"提示",QString("[%1]用户不存在，添加好友失败") .arg(strname));break;
    case user_offline:
        QMessageBox:: about(m_mainWnd,"提示",QString(" [%1]用户不在线，添加好友失败").arg(strname));break;
    case user_refuse:
        QMessageBox:: about (m_mainWnd,"提示",QString ("[%-1]用户拒绝添加你为好友，添加好友失败").arg(rs->friendName));break;
    case add_success:
        QMessageBox:: about(m_mainWnd,"提示",QString("用添加好友[31]成功").arg(rs->friendName)) ;break;
    default:
        break ;
    }
}
  //处理下线请求
void Ckernel::dealofflineRq(long iSendIp, char *buf, int nlen)
{
    qDebug ()<<__func__;
         // 1、拆包
    STRU_TCP_OFFLINE_RQ* rq = (STRU_TCP_OFFLINE_RQ* )buf;
    // 2、根据下线好友的id获取到下线好友的useritrm
    if (m_mapIdToUserItem.count (rq->userld)> 0){
    // 3、把下线好友的头像置灰
    Userlten*item = m_mapIdToUserItem [ rq->userld] ;item->setOffline () ;
    }

}
//处理关闭登录和注册页面的信号
void Ckernel::slot_close(){
    qDebug()<<__func__;
     //回收资源
    // 1、登录&注册界面指针
    if (m_loginDia){
        m_loginDia->hide ( ) ;
        delete m_loginDia;
        m_loginDia = NULL;
    }
    //2、中介者类指针
    if (m_tcpClient){
        m_tcpClient->CloseNet ();
        delete m_tcpClient;
        m_tcpClient = NULL;
    }
    //3、好友列表窗口指针
    if(m_mainWnd){
        m_mainWnd->hide () ;
        delete m_mainWnd;
        m_mainWnd = NULL;
    }
    //4、聊天窗口指针
    for (auto ite = m_mapIdToChatdlg.begin (); ite != m_mapIdToChatdlg.end();){
        chatdialog*chat = *ite;
        if (chat){
            chat->hide();
            delete chat;
            chat = NULL;
        }
        ite = m_mapIdToChatdlg.erase (ite) ;
    }
    //5、退出应用程序
    exit (0);

}
//处理关闭好友列表页面的信号
void Ckernel::slot_offline(){
    //1、给服务端发送下线请求
    STRU_TCP_OFFLINE_RQ rq;
    rq.userld = m_id;
    m_tcpClient->SendData(0,(char* ) &rq, sizeof(rq)) ;
    // 2、回收资源
    slot_close ();

}
//发送视频聊天请求
void Ckernel::slot_viderq(int id)
{
     if(videorw->u_id==-1){
        struct STRU_TCP_VIDEO rp;
        rp.userld=m_id;
        rp.friendld=id;
        m_tcpClient->SendData(0,(char*)&rp,sizeof(rp));
     }else{
         QMessageBox::about(m_mapIdToChatdlg[id],"你跟某人正在视频聊天聊天,不能在建立其他聊天","确定");
     }

}
//处理视频通话请求
void Ckernel::slot_dealVideRq(long lsendIp, char *buf, int nLen)
{
    cout <<__func__<< endl;
    STRU_TCP_VIDEO* rq = (STRU_TCP_VIDEO* )buf;
    //string f=getIpstring(lsendIp);
    //qDebug()<<(INet::getIpstring(lsendIp)).c_str();
    if(QMessageBox::Yes==QMessageBox::question(this->m_mainWnd,"视频聊天",
    (m_mapIdToChatdlg[rq->userld]->m_name),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)&&videorw->u_id==-1){
        //请求同意返回聊天回复
        if(m_mapIdToChatdlg.find(rq->userld)!=m_mapIdToChatdlg.end()){
            cout <<"derssss"<< endl;
            auRead->u_id=rq->userld;
            videorw->u_id=rq->userld;
            struct STRU_TCP_VIDEO rs;
            rs.userld=m_id;
            rs.friendld=rq->userld;
            rs.nType=_DEF_TCP_VIDEO_RS;
            m_tcpClient->SendData(0,(char*)&rs,sizeof(rs));
            chatdialog*chat=m_mapIdToChatdlg[rq->userld];
            chat->videqDialog->show();
}
    }
}
//处理视频通话回复
void Ckernel::slot_dealVideRs(long lsendIp, char *buf, int nLen)
{
   cout <<__func__<< endl;
   STRU_TCP_VIDEO* rs = (STRU_TCP_VIDEO* )buf;
   if(m_mapIdToChatdlg.find(rs->userld)!=m_mapIdToChatdlg.end()){
       chatdialog*chat=m_mapIdToChatdlg[rs->userld];
       auRead->u_id=rs->userld;
       videorw->u_id=rs->userld;
       chat->videqDialog->show();
       }
}
//发送语音聊天请求
void Ckernel::slot_voice(int id)
{
    if(auRead->u_id==-1){
        struct STRU_TCP_voice rp;
        rp.userld=m_id;
        rp.friendld=id;
        m_tcpClient->SendData(0,(char*)&rp,sizeof(rp));
    }else{
        QMessageBox::about(m_mapIdToChatdlg[id],"你跟某人正在语音聊天聊天,不能在建立其他聊天","确定");
    }
}
//处理语音通话请求
void Ckernel::deal_voicechatRp(long lsendIp, char *buf, int nLen)
{
    cout <<__func__<< endl;
    STRU_TCP_voice* rq = (STRU_TCP_voice* )buf;
    //string f=getIpstring(lsendIp);
    //qDebug()<<(INet::getIpstring(lsendIp)).c_str();
    if(QMessageBox::Yes==QMessageBox::question(this->m_mainWnd,"语音聊天",
    (m_mapIdToChatdlg[rq->userld]->m_name),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)&&auRead->u_id==-1){
        //请求同意返回聊天回复
        if(m_mapIdToChatdlg.find(rq->userld)!=m_mapIdToChatdlg.end()){
            cout <<"derssss"<< endl;
            auRead->u_id=rq->userld;
            struct STRU_TCP_voice rs;
            rs.userld=m_id;
            rs.friendld=rq->userld;
            rs.nType=_DEF_TCP_voice_RS;
            m_tcpClient->SendData(0,(char*)&rs,sizeof(rs));
            chatdialog*chat=m_mapIdToChatdlg[rq->userld];
            chat->audQDialog->show();
}
    }
}
//处理语音通话回复
void Ckernel::deal_voicechatRs(long lsendIp, char *buf, int nLen)
{
   cout <<__func__<< endl;
   STRU_TCP_voice* rs = (STRU_TCP_voice* )buf;
   if(m_mapIdToChatdlg.find(rs->userld)!=m_mapIdToChatdlg.end()){
       chatdialog*chat=m_mapIdToChatdlg[rs->userld];
       auRead->u_id=rs->userld;
       chat->audQDialog->show();

       }

}
//发送音频帧
void Ckernel::slot_audioFrame(int id,QByteArray ba){

//     struct STRU_TCP_audio_CHAT buf;
//    buf.userld = m_id;
//    buf.friendld = id;
//    //设置音频帧发送时间
//    QTime tm = QTime::currentTime();
//    buf.min = tm.minute();
//    buf.sec = tm.second();
//    buf.msec = tm.msec();
//    buf.audioFrame=ba;
//    cout <<__func__<<"发送音频数据"<<sizeof(buf)<< endl;
//    m_tcpClient->SendData( 0 , (char*)&buf , sizeof(buf));
    int nPackSize = 6*sizeof(int) + ba.size();
    char* buf = new char[nPackSize];
    char * tmp = buf;
    //序列化
    int type = _DEF_TCP_AUDIO_CHAT;

    int userId = m_id;
    int friendld = id;
    QTime tm = QTime::currentTime();
    int min = tm.minute();
    int sec = tm.second();
    int msec = tm.msec();

    *(int*)tmp = type; ////按照整型存
    tmp += sizeof(int);

    *(int*)tmp = userId;
    tmp += sizeof(int);

    *(int*)tmp = friendld;
    tmp += sizeof(int);

    *(int*)tmp = min;
    tmp += sizeof(int);

    *(int*)tmp = sec;
    tmp += sizeof(int);

    *(int*)tmp = msec;
    tmp += sizeof(int);

    memcpy( tmp , ba.data() , ba.size() );
    cout <<__func__<<"发送音频数据"<<nPackSize<< endl;
    m_tcpClient->SendData( 0 , buf , nPackSize);
    delete[] buf;
}
//给对端发送关闭语音通话通知
void Ckernel::solt_deal_audio_close(int id){
     struct Exit_voice_call rp;
     rp.userld=m_id;
     rp.friendld=id;
     m_tcpClient->SendData(0,(char*)&rp,sizeof(rp));
}
//处理对端发送关闭语音||视频通话通知
void Ckernel::deal_audio_close(long lsendIp, char *buf, int nlen){
    cout <<__func__<< "处理对端发送关闭语音||视频通话通知 "<< endl;
     Exit_voice_call *rq=(Exit_voice_call*)buf;
    if(m_mapIdToChatdlg.find(rq->userld)!=m_mapIdToChatdlg.end()){
        if(auRead->u_id!=-1){
            chatdialog*chat=m_mapIdToChatdlg[rq->userld];
            chat->audQDialog->hide();

            auRead->pause();
            auRead->u_id=-1;
        }
        if(videorw->u_id!=-1){
            chatdialog*chat=m_mapIdToChatdlg[rq->userld];
            chat->videqDialog->hide();

            videorw->slot_closeVideo();
            videorw->u_id=-1;
        }
    }

}
 //接收视频模块发送图片帧
void Ckernel::solt_sendVideoFrame(QImage img)
{
    // 压缩
    //压缩图片从RGB24格式压缩到JPEG格式, 发送出去
    QByteArray ba;
    QBuffer qbuf(&ba); // QBuffer 与 QByteArray 字节数组联立联系
    img.save( &qbuf , "JPEG" , 50 );  //将图片的数据写入 ba
    //使用ba对象, 可以获取图片对应的缓冲区
    //可以使用ba.data() , ba.size()将缓冲区发送出去

    // 写视频帧 发送
    int nPackSize = 6*sizeof(int) + ba.size();
    char * buf = new char[nPackSize];
    char* tmp = buf;

    *(int*)tmp = _DEF_TCP_VIDEO_CHAT;
    tmp +=sizeof(int);
    *(int*)tmp = m_id;
    tmp +=sizeof(int);
    *(int*)tmp = videorw->u_id;
    tmp +=sizeof(int);

    //用于延迟过久舍弃一些帧的参考时间
    QTime tm = QTime::currentTime();
    *(int*)tmp = tm.minute();
    tmp +=sizeof(int);
    *(int*)tmp = tm.second();
    tmp +=sizeof(int);
    *(int*)tmp = tm.msec();
    tmp +=sizeof(int);

    memcpy( tmp , ba.data() , ba.size() );
    /// 发送是一个阻塞函数 , 如果服务器接收缓冲区由于数据量大, 没及时取走缓冲区数据
    /// 滑动窗口变小 , send 函数阻塞, 影响用户界面响应, 出现未响应问题(卡顿)
   //m_tcpClient->SendData( 0 , buf , nPackSize );

    //delete[] buf;
     Q_EMIT SIG_SendVideo( buf , nPackSize);
}
//多线程发送视频
void Ckernel::slot_SendVideo( char* buf , int nlen){
    char* tmp = buf;
    tmp +=sizeof(int);
    tmp +=sizeof(int);
    tmp +=sizeof(int);
    //获取时间
    int min = *(int*)tmp;
    tmp +=sizeof(int);
    int sec= *(int*)tmp;
    tmp +=sizeof(int);
    int msec= *(int*)tmp;
    tmp +=sizeof(int);
    //当前时间
    QTime ctm = QTime::currentTime();
    //数据包的时间
    QTime tm( ctm.hour() , min , sec , msec );
    //发送数据包延迟超过 300ms 舍弃
    if( tm.msecsTo( ctm ) > 300  ){
        qDebug()<< "send fail" ;
        delete[] buf;
        return;
    }
    m_tcpClient->SendData( 0 , buf , nlen );
    delete[] buf;
}
//音频帧处理
void Ckernel::slot_dealAudioFrameRq(long lsendIp, char *buf, int nlen)
{
    cout <<__func__<<"处理音频数据"<<nlen<< endl;
//    //拆包
//    //音频帧
//    /// int type;
//    /// int userId;
//    /// int roomId;
//    /// int min;
//    /// int sec;
//    /// int msec;
//    /// QByteArray audioFrame;
//    STRU_TCP_audio_CHAT* rq = (STRU_TCP_audio_CHAT* )buf;
//    int nbufLen = nlen - 6*sizeof(int);
//    QByteArray ba( rq->audioFrame);
//    auWrite->slot_playAudio(ba);
    char* tmp = buf;
    int userId ;
    int friendld ;

    tmp += sizeof(int);

    userId = *(int*)tmp; //按照整型取
    tmp += sizeof(int);

    friendld = *(int*)tmp ;
    tmp += sizeof(int);

    //跳过时间
    tmp += sizeof(int);

    tmp += sizeof(int);

    tmp += sizeof(int);

    int nbufLen = nlen - 6*sizeof(int);
    QByteArray ba( tmp , nbufLen);
    auWrite->slot_playAudio(ba);

}
//视频帧处理
void Ckernel::slot_dealVideoFrameRq(long lsendIp, char *buf, int nlen)
{
    //拆包
    ///视频数据帧
    /// 成员描述
    /// int type;
    /// int userId;
    /// int roomId;
    /// int min;
    /// int sec;
    /// int msec;
    /// QByteArray videoFrame;
    char * tmp = buf;
    tmp += sizeof( int );
    int userId = *(int*) tmp;
    tmp += sizeof( int );
    int friendld = *(int*) tmp;
    tmp += sizeof( int );

    tmp += sizeof( int );
    tmp += sizeof( int );
    tmp += sizeof( int );

    int datalen = nlen - 6*sizeof(int);
    QByteArray bt(tmp , datalen);
    QImage img;
    img.loadFromData( bt );
    if(m_mapIdToChatdlg.find(userId)!=m_mapIdToChatdlg.end()){
        chatdialog*chat=m_mapIdToChatdlg[userId];
        chat->videqDialog->slot_setotherimg(img);
    }

}
