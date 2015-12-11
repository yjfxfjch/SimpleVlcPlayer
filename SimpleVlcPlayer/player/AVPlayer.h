#ifndef __AVPlayer_H__
#define __AVPlayer_H__
#include <iostream>
#include <Windows.h>

typedef void (* pfnCallback) (void *data);  // VLC事件的回调函数指针
struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_event_t;

class CAVPlayer
{
    // VLC的事件管理
    friend void OnVLC_Event(const libvlc_event_t *event, void *data);

public:
    CAVPlayer(void);
    ~CAVPlayer(void);

    bool Play(const std::string &strPath);  // 播放路径为strPath的文件
    void Play ();                           // 播放
    void Pause();                           // 暂停
    void Stop ();                           // 停止

    void Volume(int iVol);                  // 音量设置为iVol
    void VolumeIncrease();                  // 音量增大
    void VolumeReduce();                    // 音量减小 

    void SeekTo(int iPos);                  // 跳到指定位置iPos
    void SeekForward();                     // 快进
    void SeekBackward();                    // 快退

    void SetHWND(HWND hwnd);                // 设置视频显示的窗口句柄
    HWND GetHWND();                         // 获取视频显示的窗口句柄

	void SetFullScreen(bool full);			//设置全屏

    bool    IsOpen();                       // 文件是否打开
    bool    IsPlaying();                    // 文件是否正在播放
    int     GetPos();                       // 获取文件当前播放的位置
    __int64 GetTotalTime();                 // 获取总时间
    __int64 GetTime();                      // 获取时间
    int     GetVolume();                    // 获取音量

    void SetCallbackPlaying(pfnCallback pfn);         // 设置文件头读取完毕时的回调函数
    void SetCallbackPosChanged(pfnCallback pfn);      // 设置文件位置改变时的回调函数
    void SetCallbackEndReached(pfnCallback pfn);      // 设置文件头读取完毕时的回调函数

private:
    libvlc_instance_t       *m_pVLC_Inst;   // VLC实例
    libvlc_media_player_t   *m_pVLC_Player; // VLC播放器
    HWND                    m_hWnd;         // 视频显示的窗口句柄
    pfnCallback             m_pfnPlaying;   // 文件读取完毕，准备播放
    pfnCallback             m_pfnPosChanged;// 文件位置改变时的回调函数
    pfnCallback             m_pfnEndReached;// 文件播放完毕的回调函数

    void Init();                            // 初始化
    void Release();                         // 清理内存
};

#endif
