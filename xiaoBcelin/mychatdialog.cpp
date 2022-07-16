#include "mychatdialog.h"
#include "ui_mychatdialog.h"

mychatDialog::mychatDialog(QWidget *parent):QDialog(parent),ui(new Ui::mychatDialog)
{ 
    ui->setupUi(this);
    setWindowTitle("小饼通信1.0");
    //初始化垂直布局的层
    m_layout=new QVBoxLayout;
    m_layout->setContentsMargins(0,0,0,0);
    //把层设置到控件上
    ui->wdg_list->setLayout(m_layout);
    //给菜单new对象，this就是指父窗口，子控件的回收由父窗口负责
    m_menu=new QMenu(this);
    //添加菜单项
    m_menu->addAction ( "添加好友");
    m_menu->addAction ("系统设置");
    //绑定点击菜单的信号和槽函数
    connect(m_menu,SIGNAL (triggered (QAction* )) ,
    this, SLOT(slot_dealMenu (QAction*)));

}
mychatDialog::~mychatDialog()
{
    delete ui;
}
//添加好友
void mychatDialog::addFriend(Userlten *item)
{
    m_layout->addWidget(item);
}
 //设置用户信息
void mychatDialog::setInfo(QString name, QString feeling, int iconId)
{
    ui->id_name->setText(name);
    ui->le_feeling->setText(feeling);
    ui->pb_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(iconId)));
}
 //重写关闭事件
void mychatDialog::closeEvent(QCloseEvent *event)
{
    //忽略关闭事件，在kernel类中控制窗口关闭
    event->ignore();
    //弹出提示框，重复确定是否关闭
    if(QMessageBox::Yes==QMessageBox::question(this,"提示","是否关闭")){
        Q_EMIT SIG_close();
    }

}
//处理菜单点击事件
void mychatDialog::slot_dealMenu(QAction *action)
{
    if("添加好友"==action->text()){
        //给kernel发送添加好友信号
        Q_EMIT SIG_addFriend();

    }else if("系统设置"==action->text()){
        //系统处理的流程
    }
}


void mychatDialog::on_pb_menu_clicked()
{
    //获取鼠标点击的坐标位置
    QPoint p = QCursor::pos ();
    //获取菜单的高度
    QSize size = m_menu->sizeHint( ) ;
    //显示菜单
    m_menu->exec(QPoint(p.x(), p.y() - size.height () ) ) ;

}

