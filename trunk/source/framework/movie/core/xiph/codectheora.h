#ifndef maid2_framework_movie_core_xiph_codectheora_h
#define maid2_framework_movie_core_xiph_codectheora_h


#include"../../../config/define.h"
#include"../../../auxiliary/macro.h"

#include"icodec.h"

#include <theora/theora.h>


namespace Maid { namespace Movie { namespace Xiph {

  class CodecTheora
    : public ICodec
  {
  public:
    CodecTheora();
    ~CodecTheora();

    virtual void Initialize();
    virtual void Finalize();

    virtual void Decode( const OggPacket& NewPacket, SPSAMPLE& pOut );
    virtual void Skip( const OggPacket& NewPacket );

    virtual double GetTime();

    virtual double CalcPacketLength( const OggPacket& NewPacket )const;

    virtual std::string GetDebugString( const OggPacket& NewPacket )const;

    const theora_info& GetInfo() const;

    static bool IsFirstPacket( const OggPacket& FirstPacket );

  private:
    void Init( const OggPacket& NewPacket );
    void Dec( const OggPacket& NewPacket, SPSAMPLE& pOut );

    double  GetFPS() const;
    int     CalcPacketFrame( const OggPacket& NewPacket )const;

  private:
    theora_info      m_TheoraInfo;
    theora_comment   m_TheoraComment;
    theora_state     m_TheoraState;

    enum STATE
    {
      STATE_EMPTY,  //  何もしてない
      STATE_INITIALIZING, //  初期化中
      STATE_DECODING,     //  デコード中
    };

    STATE m_State;

    int     m_FrameCount;
    bool    m_IsSkipMode; //  スキップモードになった。

  };

  typedef boost::shared_ptr<CodecTheora> SPCODECTHEORA;

}}}

#endif