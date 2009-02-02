#ifndef oggdecodernull_h
#define oggdecodernull_h


#include"../../source/config/define.h"

#include"ioggdecoder.h"


class OggDecoderNULL : public IOggDecoder
{
public:
  virtual void Initialize(){}
  virtual void Decode( const OggPacket& NewPacket, SPOGGBUFFER& pDst ){}
  virtual void Finalize(){}
};

#endif
