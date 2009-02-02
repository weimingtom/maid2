#ifndef oggdecodervorbis_h
#define oggdecodervorbis_h


#include"../../source/config/define.h"
#include"../../source/auxiliary/macro.h"

#include"ioggdecoder.h"

#include <vorbis/codec.h>

class OggDecoderVorbis : public IOggDecoder
{
public:
  OggDecoderVorbis();

  virtual void Initialize();
  virtual void Decode( const OggPacket& NewPacket, SPOGGBUFFER& pDst );
  virtual void Finalize();

private:
  vorbis_info      m_Info;
  vorbis_comment   m_Comment;
  vorbis_dsp_state m_State;
  vorbis_block     m_Block;
};


#endif