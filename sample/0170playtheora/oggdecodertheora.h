#ifndef oggdecodertheora_h
#define oggdecodertheora_h


#include"../../source/config/define.h"
#include"../../source/auxiliary/macro.h"
#include"../../source/auxiliary/thread.h"

#include"ioggdecoder.h"

#include <theora/theora.h>

class OggDecoderTheora : public IOggDecoder
{
public:
  OggDecoderTheora();
  ~OggDecoderTheora();
  virtual void Initialize();
  virtual void Finalize();
  virtual void PacketIn( const OggPacket& NewPacket );
  virtual bool IsSetupped() const;

  virtual double GetTime();

  virtual void BeginSeekMode( double TargetTime );
  virtual void Flush( OGGDECODESAMPLELIST& List );

  virtual bool IsBuffering() const;

  const theora_info& GetInfo() const;

private:

  void SetPPLevel( int level );
  void OnSetup( const OggPacket& NewPacket );
  void OnDecode( const OggPacket& NewPacket );
  SPOGGDECODEDBUFFER OggDecoderTheora::CreateDecodedBuffer() const;

private:
  theora_info      m_TheoraInfo;
  theora_comment   m_TheoraComment;
  theora_state     m_TheoraState;

  int m_MaxPostProcessLevel;
  int m_CurrentPostProcessLevel;

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
};


#endif