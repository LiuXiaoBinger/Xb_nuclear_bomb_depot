
#pragma once
#include <QByteArray>
#define _DEF_UDP_PORT (43210)
#define _DEF_UDP_PORT_zijie (1024)
#define _DEF_TCP_PORT (8000)
#define _DEF_TCP_SERVER_IP ("192.168.1.104")

#define _DEF_NAME_SIZE  100
#define _DEF_MAX_SIZE  (4096)
#define _DEF_PROTOCAL_COUNT  (100)
//协议头
//协议头
#define _DEF_PROTOCAL_BASE (1000)
//注册
#define _DEF_PACK_TCP_REGISTER_RQ   (_DEF_PROTOCAL_BASE+ 1)
#define _DEF_PACK_TCP_REGISTER_RS   (_DEF_PROTOCAL_BASE + 2)
//登录
#define _DEF_PACK_TCP_LOGIN_RQ      (_DEF_PROTOCAL_BASE +3)
#define _DEF_PACK_TCP_LOGIN_RS      (_DEF_PROTOCAL_BASE +4)
//获取好友信息
#define _DEF_PACK_TCP_FRIEND_INFO   (_DEF_PROTOCAL_BASE + 5)
//添加好友
#define _DEF_PACK_TCP_ADDFRIEND_RQ  (_DEF_PROTOCAL_BASE+ 6)
#define _DEF_PACK_TCP_ADDFRIEND_RS  (_DEF_PROTOCAL_BASE+ 7)
//聊天
#define _DEF_PACK_TCP_CHAT_RQ       (_DEF_PROTOCAL_BASE + 8)
#define _DEF_PACK_TCP_CHAT_RS       (_DEF_PROTOCAL_BASE+ 9)
//下线
#define _DEF_PACK_TCP_OFFLINE_RQ    (_DEF_PROTOCAL_BASE+ 10)

//语音通话请求与回复
#define _DEF_TCP_voice_RQ           (_DEF_PROTOCAL_BASE+11)
#define _DEF_TCP_voice_RS           (_DEF_PROTOCAL_BASE+12)

//视频聊天请求与回复
#define _DEF_TCP_VIDEO_RQ           (_DEF_PROTOCAL_BASE+13)
#define _DEF_TCP_VIDEO_RS           (_DEF_PROTOCAL_BASE+14)
//音频聊天关闭
#define EXIT_VIDEO_CALL_RQ         (_DEF_PROTOCAL_BASE+16)
//视频聊天数据包
#define _DEF_TCP_VIDEO_CHAT  (_DEF_PROTOCAL_BASE+17)
//音频数据
#define _DEF_TCP_AUDIO_CHAT    (_DEF_PROTOCAL_BASE + 15)

//音频注册
#define DEF_PACK_AUDIO_REGISTER    (_DEF_PROTOCAL_BASE + 18)
//视频注册
#define DEF_PACK_VIDEO_REGISTER    (_DEF_PROTOCAL_BASE + 19)


//请求结构//注册请求结果
#define register_success (0)
#define user_is_exist    (1)
#define parameter_error   (3)
//登录请求结果
#define login_success  (0)
#define user_not_exist (1)
#define password_error (2)
//用户状态
#define status_online  (0)
#define status_offline (1)
//添加好友请求结果
#define add_success  (0)
#define no_this_user (1)
#define user_refuse  (2)
#define user_offline (3)
//聊天请求结果
#define send_success (0)
#define send_fail    (1)


//协议结构
typedef int PackType;
// 注册请求; 协议头、昵称、手机号、密码
typedef struct STRU_TCP_REGISTER_RQ{
		STRU_TCP_REGISTER_RQ():type(_DEF_PACK_TCP_REGISTER_RQ){
			memset(name,0,_DEF_NAME_SIZE);
			memset(tel,0,_DEF_NAME_SIZE);
			memset(password,0,_DEF_NAME_SIZE);
			}
			PackType type;
			char name[_DEF_NAME_SIZE]; 
			char tel[_DEF_NAME_SIZE];
			char password[_DEF_NAME_SIZE];
 } STRU_TCP_REGISTER_RQ;
// 注册回复:协议头、注册结果
typedef struct STRU_TGP_REGISTER_RS {
	STRU_TGP_REGISTER_RS() :type(_DEF_PACK_TCP_REGISTER_RS), result(register_success) {
	}
	PackType type; 
	int result;
}STRU_TGP_REGISTER_RS;

//登录请求:协议头、手机号、密码
typedef struct STRU_TCP_LOGIN_RQ{
	  STRU_TCP_LOGIN_RQ() :type(_DEF_PACK_TCP_LOGIN_RQ){
			memset(tel,0,_DEF_NAME_SIZE);
			memset(password,0,_DEF_NAME_SIZE);
		}
		PackType type;
		char tel[_DEF_NAME_SIZE];
		char password[_DEF_NAME_SIZE]; 
} STRU_TCP_LOGIN_RQ;
//登录回复:协议头、登录结果、用户id
typedef struct STRU_TCP_LOGIN_RS
{
	STRU_TCP_LOGIN_RS() :type(_DEF_PACK_TCP_LOGIN_RS), result(login_success), userld(0) {
	}
	PackType type; 
	int result; 
	int userld;
} STRU_TCP_LOGIN_RS;

//获取好友信息:协议头、用户id、昵称、签名、头像id、用户状态
typedef struct STRU_TCP_FRIEND_INFO
{
	STRU_TCP_FRIEND_INFO() :type(_DEF_PACK_TCP_FRIEND_INFO), userld(0), iconld(0), state(status_online){
		memset(name,0,_DEF_NAME_SIZE);
        memset(feeling,0,_DEF_NAME_SIZE);
}
	PackType type;
	int userld; 
	int iconld; 
	int state;
	char name[_DEF_NAME_SIZE]; 
	char feeling[_DEF_NAME_SIZE]; 
} STRU_TCP_FRIEND_INFO;

//添加好友请求:协议头、请求用户id、请求用户昵称、被请求用户昵称
typedef struct STRU_TCP_ADD_FRIEND_RQ{
	STRU_TCP_ADD_FRIEND_RQ() :type(_DEF_PACK_TCP_ADDFRIEND_RQ), userld(0) {
		memset(userName,0,_DEF_NAME_SIZE);
		memset(friendName,0,_DEF_NAME_SIZE);
	}
PackType type;
int userld;
char userName[_DEF_NAME_SIZE]; char friendName[_DEF_NAME_SIZE]; 
}STRU_TCP_ADD_FRIEND_RQ;
//添加好友回复:协议头、请求用户id、被请求用户id、被请求用户昵称、请求结果
typedef struct STRU_TCP_ADD_FRIEND_RS {
	STRU_TCP_ADD_FRIEND_RS() :type(_DEF_PACK_TCP_ADDFRIEND_RS), userld(0), friendld(0), result(add_success) {
		memset(friendName,0,_DEF_NAME_SIZE);
		}
	PackType type;
	int userld; 
	int friendld;
	int result;
	char friendName[_DEF_NAME_SIZE];
 } STRU_TCP_ADD_FRIEND_RS;

//聊天请求:协议头、用户id、好友id、聊天内容
typedef struct STRU_TCP_CHAT_RQ{
		STRU_TCP_CHAT_RQ() :type(_DEF_PACK_TCP_CHAT_RQ), userld(0), friendld(0) {
			memset(content,0,_DEF_MAX_SIZE);
		}
		PackType type;
		int userld; 
		int friendld;
char content[_DEF_MAX_SIZE]; 
} STRU_TCP_CHAT_RQ;
//聊天回复:协议头、用户id、好友id、请求结果
typedef struct STRU_TCP_CHAT_RS{
	STRU_TCP_CHAT_RS() :type(_DEF_PACK_TCP_CHAT_RS), userld(0), friendld(0), result(send_success) {
	}
	PackType type; 
	int userld;
	int friendld;
	int result; 
} STRU_TCP_CHAT_RS;

//下线请求:协议头、用户id
typedef struct STRU_TCP_OFFLINE_RQ {
	STRU_TCP_OFFLINE_RQ() :type(_DEF_PACK_TCP_OFFLINE_RQ), userld(0) {
	}
	PackType type; 
	int userld;
} STRU_TCP_OFFLINE_RQ;

//视频聊天请求与回复
struct STRU_TCP_VIDEO{
    STRU_TCP_VIDEO():nType(_DEF_TCP_VIDEO_RQ){
    }
    int nType;
    int userld;
    int friendld;
    //char content[_DEF_CONTENT_SIZE];

};
//语音通话请求与回复
struct STRU_TCP_voice{
    STRU_TCP_voice():nType(_DEF_TCP_voice_RQ){
    }
    int nType;
    int userld;
    int friendld;
    //char content[_DEF_CONTENT_SIZE];

};

//视频聊天的语音包
//   struct STRU_UDP_TCP_CHAT{
//       STRU_UDP_TCP_CHAT():nType(_DEF_UDP_AUDIO_CHAT){

//       }
//       int nType;
//       int userld;
//       int friendld;
//       //QByteArray str;
//   };
//语音通话,音频数据帧
//   struct STRU_TCP_audio_CHAT{
//       STRU_TCP_audio_CHAT():nType(_DEF_TCP_AUDIO_CHAT){
//       }
//        //成员描述
//           int nType;
//           int userld;
//           int friendld;
//           int min;
//           int sec;
//           int msec;
//           QByteArray audioFrame;//--> char frame[]; 柔性数组
//   };
   //取消语音通话连接
   struct Exit_voice_call{
       Exit_voice_call():nType(EXIT_VIDEO_CALL_RQ){

       }
       int nType;
       int userld;
       int friendld;
       //QByteArray str;
   };
   //注册音频
    struct STRU_AUDIO_REGISTER
    {
        STRU_AUDIO_REGISTER():m_nType(DEF_PACK_AUDIO_REGISTER){
            m_userid = 0;
        }

        PackType m_nType;   //包类型
        int m_userid;
    };

    //注册视频
    struct STRU_VIDEO_REGISTER
    {
        STRU_VIDEO_REGISTER():m_nType(DEF_PACK_VIDEO_REGISTER){
            m_userid = 0;
        }

        PackType m_nType;   //包类型
        int m_userid;
    };
