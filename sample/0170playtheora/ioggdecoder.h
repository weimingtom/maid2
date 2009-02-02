#ifndef ioggdecoder_h
#define ioggdecoder_h


#include"../../source/config/define.h"

#include"oggpacket.h"

#include <boost/smart_ptr.hpp>


class IOggDecoder
{
public:
  virtual ~IOggDecoder(){}

  virtual void Initialize()=0;
  virtual void Update( const OggPacket& NewPacket )=0;
  virtual void Finalize()=0;
};

typedef boost::shared_ptr<IOggDecoder> SPOGGDECODER;

#endif
