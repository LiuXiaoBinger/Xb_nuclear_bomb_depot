#ifndef AUDIOREAD_H
#define AUDIOREAD_H

#include <QObject>
#include"world.h"

class AudioRead : public QObject
{
    Q_OBJECT
public:
    explicit AudioRead(QObject *parent = 0);
public slots:
    //定时超时, 从缓冲区读取音频数据
    void ReadMore();

    void start(int id);
    void pause();
    void solt_deal_close(int id);
signals:
    void SIG_audioFrame( int id,QByteArray bt );

private:
    QAudioInput * m_audio_in;//采集声卡 音频输入
    QIODevice * m_buffer_in; //对应的缓冲区
    QTimer * m_timer;
    QAudioFormat format;
    int m_audioState;

    //SPEEX 相关变量
    SpeexBits bits_enc;
    void *Enc_State;
public:
    int u_id;
};

#endif // AUDIOREAD_H
