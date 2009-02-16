#ifndef maid2_framework_xiph_idecoder_h
#define maid2_framework_xiph_idecoder_h


#include"../../config/define.h"

#include"oggpacket.h"
#include"ibuffer.h"

#include <boost/smart_ptr.hpp>

namespace Maid { namespace Xiph {

  class IDecoder
  {
  public:
    virtual ~IDecoder(){}

    virtual void Initialize()=0;
    virtual void Finalize()=0;

    //  デコードの準備ができたか？
    virtual bool IsSetupped() const=0;

    virtual void Setup( const OggPacket& NewPacket )=0;

    //  デコード
    virtual void Decode( const OggPacket& NewPacket, SPBUFFER& pOut )=0;

    //  スキップする
    virtual void Skip( const OggPacket& NewPacket )=0;

    //  現在の時間の取得
    virtual double GetTime()=0;


    //  このパケットをデコードしたときにすすむ時間を求める
    virtual double CalcPacketLength( const OggPacket& NewPacket )const=0;

    virtual std::string GetDebugString( const OggPacket& NewPacket )const=0;
  };

  typedef boost::shared_ptr<IDecoder> SPDECODER;
}}

#endif
