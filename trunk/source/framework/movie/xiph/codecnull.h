#ifndef framework_xiph_codecnull_h
#define framework_xiph_codecnull_h


#include"../../../config/define.h"

#include"icodec.h"

namespace Maid { namespace Xiph {


  class CodecNULL
    : public ICodec
  {
  public:
    virtual void Initialize(){}
    virtual void Finalize(){}
    virtual bool IsSetupped() const{ return true; }
    virtual void Setup( const OggPacket& NewPacket ){}
    virtual void Decode( const OggPacket& NewPacket, SPSAMPLE& pOut ){}
    virtual void Skip( const OggPacket& NewPacket ){}
    virtual double GetTime(){return 0.0;}
    virtual double CalcPacketLength( const OggPacket& NewPacket )const{return 0.0;}
    virtual std::string GetDebugString( const OggPacket& NewPacket )const{ return std::string(); }
  };

}}

#endif
