#ifndef ioggdecoder_h
#define ioggdecoder_h


#include"../../source/config/define.h"

#include"oggstream.h"
#include"oggpacket.h"

class IOggDecoder
{
public:
  virtual ~IOggDecoder(){}

  virtual void Initialize( const OggPacket& NewPacket )=0;
  virtual void Update( const OggPacket& NewPacket )=0;
  virtual void Finalize()=0;
};


#endif
