#ifndef USERLTEN_H
#define USERLTEN_H

#include <QDialog>

namespace Ui {
class Userlten;
}

class Userlten : public QDialog
{
    Q_OBJECT
signals:
    //用户点击好友头像的信号
    void SIG_UserItemClicked(int id);
public:
    explicit Userlten(QWidget *parent = nullptr);
    ~Userlten();
    //设置好友信息
    void setInfo(QString name,QString feeling,int state,int icon,int id);
    //头像置灰
    void setOffline();
public:
    int m_state;
    QString m_name;
private slots:
    void on_bp_icon_clicked();

private:
    Ui::Userlten *ui;

    QString m_feeling;

    int m_icon;
    int m_id;
};

#endif // USERLTEN_H
