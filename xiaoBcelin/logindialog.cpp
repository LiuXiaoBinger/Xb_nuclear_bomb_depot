#include "logindialog.h"
#include "ui_logindialog.h"
#include<QMessageBox>
#include<iostream>

#include<QMessageBox>
loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    ui->setupUi(this);
}

loginDialog::~loginDialog()
{
    delete ui;
}
//重写关闭事件
void loginDialog::closeEvent(QCloseEvent *event)
{
    event->accept();
    Q_EMIT SIG_close();
}

void loginDialog::on_pb_clear_clicked()
{

   // std::cout<<__func__<<std::endl;
    ui->le_password->setText("");
    ui->le_tel->setText("");
}


void loginDialog::on_pb_commit_clicked()
{
    //1、获取控件上的数据

    QString tel=ui->le_tel->text();
    QString password=ui->le_password->text();
    //2、校验数据的合法性
    //2.1、2.1、长度:都不能为空，昵称不能超过10个字符，电话号必须是11位，密码不能超过15个字符
    //2.2、内容:昵称不允许有特殊字符、申话号码必须要都是数字、密码只能由字母、数字、下滑线组成
    QString passwordTmp=password;
    if (tel.isEmpty() || password.isEmpty() ||
        passwordTmp.remove (" ") .isEmpty()){
        QMessageBox ::about(this,"提示","输入内容不能为空或者是全空格");
        return;
    }
    if ( tel.length() != 11 || password.length() > 15){
        QMessageBox :: about(this,"提示","输入内容长度不合法");
        return;
    }
    // 3、把数据发给kernel
    Q_EMIT SIG_LoginCommit ( tel, password);
}


void loginDialog::on_pb_clear_register_clicked()
{


   // std::cout<<__func__<<std::endl;
   ui->le_name_register->setText("");
   ui->le_tel_register->setText("");
   ui->le_password_register->setText("");
}


void loginDialog::on_pb_connit_register_clicked()
{
    //1、获取控件上的数据
    QString name=ui->le_name_register->text();
    QString tel=ui->le_tel_register->text();
    QString password=ui->le_password_register->text();
    //2、校验数据的合法性
    //2.1、2.1、长度:都不能为空，昵称不能超过10个字符，电话号必须是11位，密码不能超过15个字符
    //2.2、内容:昵称不允许有特殊字符、申话号码必须要都是数字、密码只能由字母、数字、下滑线组成
    QString passwordTmp=password;
    if (name . isEmpty() || tel.isEmpty() || password.isEmpty() ||
        passwordTmp.remove (" ") .isEmpty()){
        QMessageBox ::about(this,"提示","输入内容不能为空或者是全空格");
        return;
    }
    if (name.length() > 10 || tel.length() != 11 || password.length() > 15){
        QMessageBox :: about(this,"提示","输入内容长度不合法");
        return;
    }
    // 3、把数据发给kernel
    Q_EMIT SIG_RegisterCommit (name, tel, password) ;

}

