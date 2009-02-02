#ifndef ioggdecodernull_h
#define ioggdecodernull_h


#include"../../source/config/define.h"

#include"ioggdecoder.h"


class OggDecoderNULL
{
public:
  virtual void Initialize(){}
  virtual void Update( const OggPacket& NewPacket ){}
  virtual void Finalize(){}
};

#endif
