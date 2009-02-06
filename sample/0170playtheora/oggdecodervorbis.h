#ifndef oggdecodervorbis_h
#define oggdecodervorbis_h


#include"../../source/config/define.h"
#include"../../source/auxiliary/macro.h"
#include"../../source/auxiliary/thread.h"
#include"../../source/sound/common.h"

#include"ioggdecoder.h"

#include <vorbis/codec.h>

class OggDecoderVorbis : public IOggDecoder
{
public:
  OggDecoderVorbis();
  ~OggDecoderVorbis();

  virtual void Initialize();
  virtual void Finalize();

  virtual void PacketIn( const OggPacket& NewPacket );
  virtual bool IsSetupped() const;

  virtual double GetTime();

  virtual void BeginSeekMode( double TargetTime );
  virtual void Flush( OGGDECODESAMPLELIST& List );

  virtual bool IsBuffering() const;

  const vorbis_info& GetInfo() const;

  Maid::PCMFORMAT GetFormat();

private:
  void OnSetup( const OggPacket& NewPacket );
  void OnDecode( const OggPacket& NewPacket );
  SPOGGDECODEDBUFFERPCM CreateDecodedBuffer();

private:
  vorbis_info      m_VorbisInfo;
  vorbis_comment   m_VorbisComment;
  vorbis_dsp_state m_VorbisState;
  vorbis_block     m_VorbisBlock;

  enum STATE
  {
    STATE_EMPTY,  //  何もしてない
    STATE_INITIALIZING, //  初期化中
    STATE_DECODING,     //  デコード中
  };

  STATE m_State;

  double  m_DecodeStartTime;  //  デコードを開始する時間

  Maid::ThreadMutex m_Mutex;
  OGGDECODESAMPLELIST  m_BufferingList;

  ogg_int64_t m_DecodeSample; //  デコードしたバイト数

};


#endif