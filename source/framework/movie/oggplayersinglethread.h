﻿#ifndef maid2_framework_oggplayersinglethread_h
#define maid2_framework_oggplayersinglethread_h

#include"../config/define.h"

#include"../auxiliary/thread.h"
#include"../auxiliary/string.h"
#include"../auxiliary/buffer.h"
#include"../auxiliary/timer.h"
#include"../sound/common.h"

#include"xiph/oggcontainer.h"
#include"oggdecodersinglethread.h"
#include"oggtimer.h"
#include"xiph/decodertheora.h"
#include"xiph/decodervorbis.h"


#include<map>
#include<list>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class OggPlayerSingleThread
  {
  public:
    OggPlayerSingleThread();
    ~OggPlayerSingleThread();

    void Initialize( const String& FileName );
    void Finalize();

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

    void Update();

    void Play();
    void FlushImage( Xiph::SPBUFFERIMAGE& pOutput );
    void FlushPCM( Buffer& Output );

    void Seek( double time );

    bool IsCacheFull() const;
    bool IsEnd() const;

    double GetPosition() const;

  private:

    bool IsSetSeek()const;

  private:
    Xiph::OggContainer m_Buffer;


   #pragma COMPILERMSG("TODO:コピー処理を書いてないのでスマポにしておく。そのうちなおす")	
    typedef boost::shared_ptr<OggDecoderSingleThread> SPDECODER;
    typedef std::map<int,SPDECODER> BINDDATA;
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

    OggTimer  m_Timer;

  private:
    void PageSeek(int Serial );
    bool IsCacheFull( int DecoderSerial )const;
    bool IsEnd( int DecoderSerial )const;

  private:
    void InitializeStream( const String& FileName );
    void FinalizeStream();
    void PlayDecode();
  };
}

#endif