#ifndef VIDEDIALOG_H
#define VIDEDIALOG_H
#include <QImage>
#include <QDialog>
#include<QMessageBox>
namespace Ui {
class videDialog;
}

class videDialog : public QDialog
{
    Q_OBJECT

public:
    explicit videDialog(QWidget *parent = nullptr);
    ~videDialog();
signals:
    void SIG_START(int id);
    void SIG_STOP();
    void SIG_screenStart();
    void SIG_screenPause();
    void SIG_close(int id);
public:
    //重写关闭事件
    void closeEvent(QCloseEvent*event);
    QString name;
    int u_id;
    //设置聊天窗口信息
    void setInfo();
public slots:
    //设置图片到朋友视频窗口
    void slot_setotherimg(QImage img);
    //设置自己图像
    void slot_setmyimg(QImage img);

private slots:
    void on_stat_vide_clicked();

    void on_stop_vide_clicked();

    void on_cb_desks_start_clicked();

    void on_cb_desks_stop_clicked();

private:
    Ui::videDialog *ui;
};

#endif // VIDEDIALOG_H
