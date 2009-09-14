#ifndef maid2_framework_movieplayer_h
#define maid2_framework_movieplayer_h

#include"../config/define.h"

#include"../auxiliary/thread.h"
#include"../auxiliary/string.h"
#include"../auxiliary/memorybuffer.h"
#include"../auxiliary/timer.h"
#include"../sound/common.h"

#include"xiph/oggcontainer.h"
#include"streamdecodermultithread.h"
#include"mediatimer.h"


#include<map>
#include<list>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class MoviePlayer
  {
  public:
    MoviePlayer();

    void Initialize( const String& FileName );

    bool IsInitialized() const;
    bool IsSeeking() const;

    struct FILEINFO
    {
      struct IMAGE
      {
        SIZE2DI EncodedSize;    //  データとして入っている大きさ
        SIZE2DI DisplaySize;    //  表示しなくてはいけない大きさ
        POINT2DI DisplayOffset; //  配置座標
        int FpsNumerator;       //  FPS の分子
        int FpsDenominator;     //  FPS の分母
        int AspectNumerator;    //  アスペクト の分子
        int AspectDenominator;  //  アスペクト の分母

        enum PIXELFORMAT
        {
          PIXELFORMAT_YUV444,
          PIXELFORMAT_YUV422,
          PIXELFORMAT_YUV420,
        };

        PIXELFORMAT PixelFormat;
      };
      bool IsImage; //  画像があるか？
      IMAGE  Image;

      struct PCM
      {
        PCMFORMAT Format;
      };

      bool IsPCM; //  サウンドがあるか？
      PCM  Pcm;

      FILEINFO()
        :IsImage(false)
        ,IsPCM(false)
      {
      }
    };

    const FILEINFO& GetFileInfo() const;


    void Play();
    void Stop();
    void Resume();

    void FlushImage( double& time, SPSAMPLEIMAGE& pOutput );
    void FlushPCM( double& time, MemoryBuffer& Output );

    void Seek( double time );

    bool IsCacheFull() const;
    bool IsEnd() const;

    double GetPosition() const;

  private:

    bool IsSetSeek()const;

  private:
    Xiph::OggContainer m_Buffer;


    typedef std::map<int,SPSTREAMDECODERMULTITHREAD> BINDDATA;
    BINDDATA m_BindData;  //  こいつにアクセスするとき同期を取ってないんだけど
                          //  STATE_INITIALIZING 以降は変更を加えることがないのでいらないです。

    enum
    {
      DECODER_EMPTY = -1,
    };


    int m_TheoraSerial;
    int m_VorbisSerial;


    enum STATE
    {
      STATE_EMPTY,
      STATE_INITIALIZING,
      STATE_SEEKING,
      STATE_WORKING,
      STATE_FINALIZING,
    };

    STATE m_State;

    double m_SeekPosition;  //  シーク命令が出たときに設定される値（単位は秒）シークしないなら負数

    String    m_FileName;
    FILEINFO  m_FileInfo;

    MediaTimer  m_Timer;

  private:
    void PageSeek( volatile ThreadController::BRIGEDATA& brige, int Serial );
    void Sleep( volatile ThreadController::BRIGEDATA& brige );
    bool IsCacheFull( int DecoderSerial )const;
    bool IsSeeking( int DecoderSerial )const;
    bool IsEnd( int DecoderSerial )const;

  private:
    unt ThreadFunction( volatile ThreadController::BRIGEDATA& state );
    void InitializeStream( const String& FileName );
    void FinalizeStream();
    void PlayDecode( volatile ThreadController::BRIGEDATA& state );
    ThreadController  m_Thread;
  };
}

#endif
