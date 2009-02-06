#ifndef oggdecodernull_h
#define oggdecodernull_h


#include"../../source/config/define.h"

#include"ioggdecoder.h"


class OggDecoderNULL : public IOggDecoder
{
public:
  OggDecoderNULL(){}
  virtual void Initialize(){}
  virtual void Finalize(){}
  virtual void PacketIn( const OggPacket& NewPacket ){}
  virtual bool IsSetupped() const { return true; }

  virtual double GetTime() { return 0; }

  virtual void BeginSeekMode( double TargetTime ) { }
  virtual void Flush( OGGDECODESAMPLELIST& List ){}

  virtual bool IsBuffering() const { return false; }

};

#endif
