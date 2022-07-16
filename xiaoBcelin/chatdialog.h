#ifndef CHATDIALOG_H
#define CHATDIALOG_H
#include<QTime>
#include <QWidget>
//#include"video.h"
//#include"voice.h"
#include"AudioApi/audioread.h"
#include"AudioApi/audiowrite.h"
#include"audioreadqdialog.h"
#include"videdialog.h"
namespace Ui {
class chatdialog;
}

class chatdialog : public QWidget
{
    Q_OBJECT
signals:
    //发送聊天数据给kernel
    void SIG_sendChatMsg (int ip,QString content);
    //发送信号给kernel，请求视频聊天
    void SIG_vide (int ip);
    //发送信号给kernel，请求语音通话
    void SIG_voice (int ip);
public:
    explicit chatdialog(QWidget *parent = 0);
    ~chatdialog();
    //设置聊天窗口信息
    void setInfo(QString name,int id);
    //设置聊天内容
    void setChatContent(QString content);
    //设置用户不在线
    void setUserOffline();

private slots:
    void on_pb_send_clicked();

//    void on_pb_vide_clicked();

    //发送语音通话请求，点击槽函数
    void on_pb_voice_clicked();

    void on_pb_vide_clicked();

private:
    Ui::chatdialog *ui;
    int m_id;//好友id





public:
//    video *Myvideo;
//    Voice *Myvoice;
    audioreadQDialog*audQDialog;
    videDialog* videqDialog;
      QString m_name;
};

#endif // CHATDIALOG_H



