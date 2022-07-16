#include "userlten.h"
#include "ui_userlten.h"
#include<QBitmap>
#include<qDebug>
Userlten::Userlten(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Userlten)
{
    ui->setupUi(this);
}

Userlten::~Userlten()
{
    delete ui;
}
 //设置好友信息
void Userlten::setInfo(QString name, QString feeling, int state, int icon, int id)
{
    //1、保存好友信息
    m_name = name;
    m_feeling = feeling;
    m_state =state;
    m_icon =icon;
    m_id =id;
    //2、把好友信息显示到界面上
    ui->ib_name->setText (m_name) ;
    ui->ib_feeling->setText (m_feeling) ;
    //设置好友头像，根据状态判断，显示彩色还是灰色的头像
    QString iconPath = QString (":/tx/%1.png" ) .arg (m_icon);
    qDebug () <<"iconPath : " << iconPath;
    if (m_state) {
    //在线，显示彩色头像
    ui->bp_icon->setIcon (QIcon(iconPath) );
    }
    else {
    //不在线，显示灰色头像
        QBitmap bmp;
        bmp . load (iconPath) ;
        ui->bp_icon->setIcon (bmp) ;
    }
    //3、重绘
    this->repaint ( );

}

void Userlten::setOffline()
{
    QString iconPath = QString ( ":/tx/%1.png" ) .arg(m_icon) ;
    qDebug () << "iconPath:" << iconPath;
    //设置灰色头像
    QBitmap bmp;
    bmp.load(iconPath);
    ui->bp_icon->setIcon (bmp);

    // 3、重绘
    this->repaint ();

}

void Userlten::on_bp_icon_clicked()
{
    //发送点击好友列表信号给kernel
    Q_EMIT SIG_UserItemClicked(m_id);
}

