#ifndef VIDEOREAD_H
#define VIDEOREAD_H

#include <QObject>
#include <QImage>
#include<QTimer>
#include"common.h"

//图片的宽高
#define IMAGE_WIDTH (320)
#define IMAGE_HEIGHT (240)


class VideoRead : public QObject
{
    Q_OBJECT
public:
    explicit VideoRead(QObject *parent = nullptr);
    ~VideoRead();
signals:
    void SIG_sendVideoFrame(QImage img);
public slots:
    void slot_getVideoFrame();
    void slot_openVideo(int id);
    void slot_closeVideo();
    //处理窗口❌函数
    void solt_deal_close(int id);
private:
    QTimer *m_timer;

    //opencv获取图片的对象
    cv::VideoCapture cap;
public:
    int u_id;
};

#endif // VIDEOREAD_H
