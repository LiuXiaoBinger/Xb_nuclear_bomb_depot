#include "videoread.h"
#include<QMessageBox>
#include<QDebug>


VideoRead::VideoRead(QObject *parent) : QObject(parent)
{
    m_timer = new QTimer;
    connect( m_timer , SIGNAL(timeout())
             , this , SLOT( slot_getVideoFrame()) );
    u_id=-1;
}

VideoRead::~VideoRead()
{
    if( m_timer )
    {
        m_timer->stop();
        delete m_timer; m_timer = NULL;
    }
}

//摄像头采集图像流程,发送图片
void VideoRead::slot_getVideoFrame()
{
    //从摄像头对象读取数据
    cv::Mat frame; // Mat 对象存储图像
    if( !cap.read(frame) )
    {
        return;
    }
    // Mat 图像不是显示和传输的格式(RGB)
    // 需要格式转换 opencv 采集的格式BGR  显示格式 RGB
    //将opencv采集的BGR的图片类型转化为RGB24的类型
    cvtColor(frame,frame,CV_BGR2RGB);
    //定义QImage对象, 用于发送数据以及图片显示
    QImage image((unsigned const char*)frame.data,frame.cols,frame.rows,QImage::Format_RGB888);
    //转化为大小更小的图片  保持缩放比
    image = image.scaled( IMAGE_WIDTH,IMAGE_HEIGHT, Qt::KeepAspectRatio );
    //发送图片
    Q_EMIT SIG_sendVideoFrame( image );

}

void VideoRead::slot_openVideo(int id)
{
    if(u_id==-1)u_id=id;
    m_timer->start( 1000/FRAME_RATE - 10 );
    //打开摄像头
    cap.open(0); // 0 默认摄像头
    if(!cap.isOpened()){
        QMessageBox::information(NULL,tr("提示"),tr("视频没有打开"));
        return;
    }

}

void VideoRead::slot_closeVideo()
{

    m_timer->stop();
    //关闭摄像头
    if(cap.isOpened())
        cap.release();
}

void VideoRead::solt_deal_close(int id){
    if(u_id==id){
        slot_closeVideo();
        u_id=-1;
    }
}
