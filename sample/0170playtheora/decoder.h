#ifndef decoder_h
#define decoder_h

#include"../../source/auxiliary/thread.h"
#include"../../source/auxiliary/string.h"
#include"../../source/auxiliary/buffer.h"
#include"../../source/graphics/surface.h"
#include"../../source/sound/common.h"

#include"oggcontainer.h"
#include"oggstream.h"
#include"ioggdecoder.h"
#include"oggdecodertheora.h"
#include"oggdecodervorbis.h"


#include<map>
#include<list>
#include<boost/smart_ptr.hpp>


class Decoder
{
public:
  Decoder();

  void Initialize();

  bool IsInitialized() const;
  bool IsSeeking() const;

  struct FILEINFO
  {
    struct IMAGE
    {
      Maid::SIZE2DI EncodedSize;    //  データとして入っている大きさ
      Maid::SIZE2DI DisplaySize;    //  表示しなくてはいけない大きさ
      Maid::POINT2DI DisplayOffset; //  配置座標
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
      Maid::PCMFORMAT Format;
    };

    bool IsPCM; //  サウンドがあるか？
    PCM  Pcm;
  };

  void GetFileInfo( FILEINFO& Info ) const;


  void FlushImage( double time, SPOGGDECODEDBUFFERIMAGE& pOutput );

  void FlushPCM( double time, Maid::Buffer& WaveData );

  void Seek( double time );

  bool IsEnd() const;

private:

  bool IsSetSeek()const;

private:
  OggContainer m_Buffer;

  struct DATA
  {
    OggStream     Stream;
    SPOGGDECODER  pDecoder;
  };

 #pragma COMPILERMSG("TODO:コピー処理を書いてないのでスマポにしておく。そのうちなおす")	
  typedef std::map<int,boost::shared_ptr<DATA> > BINDDATA;
  BINDDATA m_BindData;

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

private:
  void Sleep( volatile Maid::ThreadController::BRIGEDATA& brige );

  OGGDECODESAMPLELIST m_ImageChache;
private:
  unt ThreadFunction( volatile Maid::ThreadController::BRIGEDATA& state );
  void InitializeStream( const Maid::String& FileName );
  void FinalizeStream();
  void SeekStream( double time, volatile Maid::ThreadController::BRIGEDATA& state );
  void PlayDecode( double begintime, volatile Maid::ThreadController::BRIGEDATA& state );
  Maid::ThreadController  m_Thread;
};

#endif
