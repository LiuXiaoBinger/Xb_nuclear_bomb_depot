#ifndef MYCHATDIALOG_H
#define MYCHATDIALOG_H

#include <QBoxLayout>
#include <QMenu>
#include "userlten.h"
#include <QDialog>
#include <QCloseEvent>
#include<QMessageBox>

namespace Ui { class mychatDialog; }


class mychatDialog : public QDialog
{
    Q_OBJECT
signals:
    //给kernel发送添加好友信号
    void SIG_addFriend();
    //关闭登录和注册页面的信号
    void SIG_close();
public:
    mychatDialog(QWidget *parent = nullptr);
    ~mychatDialog();
    //添加好友
    void addFriend(Userlten*item);

    //设置用户信息
    void setInfo(QString name ,QString feeling,int iconId);
    //重写关闭事件
    void closeEvent(QCloseEvent *event);
private slots:
    //void on pb_ menu_ clicked ( ) ;
    //处理菜单点击事件
    void slot_dealMenu(QAction* action) ;

    void on_pb_menu_clicked();

private:
    Ui::mychatDialog *ui;
    QVBoxLayout* m_layout;

    QMenu*m_menu;
};
#endif // MYCHATDIALOG_H
