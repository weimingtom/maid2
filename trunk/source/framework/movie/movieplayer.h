#ifndef maid2_framework_movie_movieplayer_h
#define maid2_framework_movie_movieplayer_h

#include"../../config/define.h"
#include"../../auxiliary/mathematics.h"

#include"../../auxiliary/thread.h"
#include"../../auxiliary/string.h"
#include"../../auxiliary/memorybuffer.h"
#include"../../sound/common.h"

#include"mediatimer.h"
#include"core/isample.h"
#include"core/istoragereader.h"
#include"core/idecodermanager.h"


namespace Maid
{
  class MoviePlayer
  {
  public:
    MoviePlayer();

    void Initialize( const String& FileName );

    bool IsStandby() const;

    struct FILEINFO
    {
      struct FRAME
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
      FRAME  Image;

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

    void FlushImage( double& time, Movie::SPSAMPLEFRAME& pOutput );
    void FlushPCM( double& time, MemoryBuffer& Output );

    bool IsCacheFull() const;
    bool IsEnd() const;

    double GetPosition() const;
    void SetPosition( double time );

  private:
    String    m_FileName;
    FILEINFO  m_FileInfo;

    MediaTimer  m_Timer;

    enum STATE
    {
      STATE_EMPTY,        //  インスタンスが生成されただけ
      STATE_INITIALIZING, //  初期化中
      STATE_SEEKING,      //  シーク中
      STATE_WORKING,      //  稼動中
      STATE_END,          //  再生終了
    };

    STATE m_State;

    Movie::SPSTORAGEREADER  m_pStorage;
    Movie::SPDECODERMANAGER m_pManager;

  private:
    void Init(volatile ThreadController::BRIGEDATA& state);
    void Seek(volatile ThreadController::BRIGEDATA& state);
    void Work(volatile ThreadController::BRIGEDATA& state);

    void Update();
    ThreadMutex m_ManagerMutex;

  private:
    unt ThreadFunction( volatile ThreadController::BRIGEDATA& state );
    ThreadController  m_Thread;
  };
}

#endif
