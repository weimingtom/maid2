#ifndef maid2_framework_xiph_codecvorbis_h
#define maid2_framework_xiph_codecvorbis_h


#include"../../../config/define.h"
#include"../../../auxiliary/macro.h"
#include"../../../auxiliary/thread.h"
#include"../../../sound/common.h"

#include"icodec.h"

#include <vorbis/codec.h>

namespace Maid { namespace Xiph {




  class CodecVorbis 
    : public ICodec
  {
  public:
    CodecVorbis();
    ~CodecVorbis();

    virtual void Initialize();
    virtual void Finalize();

    virtual bool IsSetupped() const;

    virtual void Setup( const OggPacket& NewPacket );
    virtual void Decode( const OggPacket& NewPacket, SPSAMPLE& pOut );
    virtual void Skip( const OggPacket& NewPacket );

    virtual double GetTime();

    virtual double CalcPacketLength( const OggPacket& NewPacket )const;

    const vorbis_info& GetInfo() const;

    PCMFORMAT GetFormat();

    virtual std::string GetDebugString( const OggPacket& NewPacket )const;

  private:
    vorbis_info      m_VorbisInfo;
    vorbis_comment   m_VorbisComment;
    vorbis_dsp_state m_VorbisState;
    vorbis_block     m_VorbisBlock;

    enum STATE
    {
      STATE_EMPTY,  //  何もしてない
      STATE_INITIALIZING, //  初期化中
      STATE_DECODING,     //  デコード中
    };

    STATE m_State;
  };

}}

#endif