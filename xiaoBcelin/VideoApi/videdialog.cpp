#include "videdialog.h"
#include "ui_videdialog.h"

videDialog::videDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::videDialog)
{
    ui->setupUi(this);
}

videDialog::~videDialog()
{
    delete ui;
}
 //设置图片到朋友视频窗口
void videDialog::slot_setotherimg(  QImage img)
{
    QPixmap pix = QPixmap::fromImage( img );
    ui->other_vide->setPixmap( pix );
    //更新
    ui->other_vide->update();
}

//设置自己图像
void videDialog::slot_setmyimg(QImage img){
    QPixmap pix = QPixmap::fromImage( img );
    ui->my_vide->setPixmap( pix );
    //更新
    ui->my_vide->update();
}
//重写关闭事件
void videDialog::closeEvent(QCloseEvent*event){
    //弹出一个询问窗口，是否关闭
       if(QMessageBox::Yes==QMessageBox::question(this,"提示","是否关闭")){
          this->hide();
          //发送关闭信号给kernnel类
          Q_EMIT SIG_close(u_id);
       }
}

void videDialog::setInfo()
{
    setWindowTitle(QString("与【%1】的聊天窗口").arg(name));
}

void videDialog::on_stat_vide_clicked()
{
     Q_EMIT SIG_screenPause();
     Q_EMIT SIG_START(u_id);

}


void videDialog::on_stop_vide_clicked()
{
    Q_EMIT SIG_STOP();
}


void videDialog::on_cb_desks_start_clicked()
{
    Q_EMIT SIG_STOP();
    Q_EMIT SIG_screenStart();
}


void videDialog::on_cb_desks_stop_clicked()
{
     Q_EMIT SIG_screenPause();
}

