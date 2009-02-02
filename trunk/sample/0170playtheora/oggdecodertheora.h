#ifndef oggdecodertheora_h
#define oggdecodertheora_h


#include"../../source/config/define.h"
#include"../../source/auxiliary/macro.h"
#include"../../source/graphics/surface.h"
#include"../../source/auxiliary/timer.h"

#include"ioggdecoder.h"

#include <theora/theora.h>
#include <list>
#include <boost/smart_ptr.hpp>

class OggDecoderTheora : public IOggDecoder
{
public:
  OggDecoderTheora();
  virtual void Initialize();
  virtual void Decode( const OggPacket& NewPacket, SPOGGBUFFER& pDst );
  virtual void Finalize();

  bool IsSetupped() const;
  const theora_info& GetInfo() const;

private:

  void OnSetup( const OggPacket& NewPacket );
  void OnDecode( const OggPacket& NewPacket, SPOGGBUFFER& pDst );

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
    STATE_WORKING,      //  デコードできまっせ！
  };

  STATE m_State;

};


#endif