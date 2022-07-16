#include "audioreadqdialog.h"
#include "ui_audioreadqdialog.h"
#include<QMessageBox>
audioreadQDialog::audioreadQDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::audioreadQDialog)
{
    ui->setupUi(this);
}

audioreadQDialog::~audioreadQDialog()
{
    delete ui;
}
//聊天音频开启
void audioreadQDialog::on_aud_start_clicked()
{
    Q_EMIT SIG_START(u_id);
}

//设置聊天窗口信息
void audioreadQDialog::setInfo()
{
   setWindowTitle(QString("与【%1】的聊天窗口").arg(name));
}
void audioreadQDialog::closeEvent(QCloseEvent *event)
{
    //弹出一个询问窗口，是否关闭
    if(QMessageBox::Yes==QMessageBox::question(this,"提示","是否关闭")){
       this->hide();
       //发送关闭信号给kernnel类
       Q_EMIT SIG_close(u_id);

    }
//    else{
//        //this->ignore();
//    }
}
//聊天音频暂停
void audioreadQDialog::on_aud_stop_clicked()
{
     Q_EMIT SIG_STOP();
}

