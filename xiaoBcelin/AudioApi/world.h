#ifndef WORLD_H
#define WORLD_H

#include<QAudioInput>
#include<QAudioOutput>
#include<QIODevice>
#include<QTimer>
#include<QMessageBox>

enum ENUM_PLAY_STATE { stopped , playing , pausing };

#include"speex/include/speex.h"

#define SPEEX_QUALITY (8)

#define USE_SPEEX   (1)

#endif // WORLD_H
