#include "chatdialog.h"
#include "ui_chatdialog.h"
#include<iostream>
#include"INet.h"
using namespace std;
chatdialog::chatdialog(QWidget *parent) :audQDialog(nullptr),
    QWidget(parent),
    ui(new Ui::chatdialog)/*,Myvideo(nullptr),Myvoice(nullptr)*/
{
    ui->setupUi(this);
    audQDialog=new audioreadQDialog;
    videqDialog=new videDialog;

}

chatdialog::~chatdialog()
{
    delete ui;
}
 //设置聊天窗口信息
void chatdialog::setInfo(QString name,int id)
{
    m_id=id;
    m_name=name;
    audQDialog->name=name;
    audQDialog->setInfo();
    audQDialog->u_id=id;
    videqDialog->u_id=m_id;
    videqDialog->name=m_name;
    videqDialog->setInfo();

    setWindowTitle(QString("与【%1】的聊天窗口").arg(m_name));
}
//设置聊天内容
void chatdialog::setChatContent(QString content)
{
    ui->tb_chat->append(QString("[%1] %2").arg(m_name).arg(QTime::currentTime().toString("hh:mm:ss")));
    ui->tb_chat->append(content);
}
//设置用户不在线
void chatdialog::setUserOffline()
{
    ui->tb_chat->append(QString("[%1] %2 该用户不在线").arg(QTime::currentTime().toString("hh:mm:ss")));
}

void chatdialog::on_pb_send_clicked()
{
    //1、先获取输入内容，校验输入内容是否合法
    QString content=ui->te_chat->toPlainText();
    if(content.isEmpty()){
        return;
    }
    //2、清空编辑窗口的内容
    content =ui->te_chat->toHtml();//获取带格式文本
    ui->te_chat->clear();

    //把内容显示到浏览窗口，格式“【我】 时间 （换行）内容"
    ui->tb_chat->append(QString("【我】 %1").arg(QTime::currentTime().toString("hh:mm:ss")));
    ui->tb_chat->append(content);

    //4、把内容发给kernel
    Q_EMIT SIG_sendChatMsg(m_id,content);
}


////发送视频聊天请求，点击槽函数
//void chatDialog::on_pb_vide_clicked()
//{

//    cout<<__func__<<endl;
//    Q_EMIT SIG_vide (inet_addr(m_ip.toStdString().c_str()));
//}

////发送语音通话请求，点击槽函数
//void chatDialog::on_pb_voice_clicked()
//{
//    Q_EMIT SIG_voice (inet_addr(m_ip.toStdString().c_str()));
//}

//发送语音通话请求，点击槽函数
void chatdialog::on_pb_voice_clicked()
{
    cout<<__func__<<endl;
    Q_EMIT SIG_voice (m_id);
}
//视频聊天点击槽函数
void chatdialog::on_pb_vide_clicked()
{
    cout<<__func__<<endl;
    Q_EMIT SIG_vide (m_id);
}

