#ifndef AUDIOREADQDIALOG_H
#define AUDIOREADQDIALOG_H

#include <QDialog>

namespace Ui {
class audioreadQDialog;
}

class audioreadQDialog : public QDialog
{
    Q_OBJECT

signals:
    void SIG_START(int id);
    void SIG_STOP();
    void SIG_close(int id);
public:
    explicit audioreadQDialog(QWidget *parent = 0);
    ~audioreadQDialog();
    //重写关闭事件
    void closeEvent(QCloseEvent*event);
    QString name;
    int u_id;
    //设置聊天窗口信息
    void setInfo();
private slots:
    void on_aud_start_clicked();
    void on_aud_stop_clicked();

private:
    Ui::audioreadQDialog *ui;
};

#endif // AUDIOREADQDIALOG_H
