#ifndef maid2_framework_xiph_codectheora_h
#define maid2_framework_xiph_codectheora_h


#include"../../../config/define.h"
#include"../../../auxiliary/macro.h"
#include"../../../auxiliary/thread.h"

#include"icodec.h"

#include <theora/theora.h>


namespace Maid { namespace Xiph {



  class CodecTheora
    : public ICodec
  {
  public:
    CodecTheora();
    ~CodecTheora();
    virtual void Initialize();
    virtual void Finalize();

    virtual bool IsSetupped() const;

    virtual void Setup( const OggPacket& NewPacket );
    virtual void Decode( const OggPacket& NewPacket, SPSAMPLE& pOut );
    virtual void Skip( const OggPacket& NewPacket );

    virtual double GetTime();

    virtual double CalcPacketLength( const OggPacket& NewPacket )const;

    virtual std::string GetDebugString( const OggPacket& NewPacket )const;

    const theora_info& GetInfo() const;
    int CalcPacketFrame( const OggPacket& NewPacket )const;

  private:
    double GetFPS() const;
    void SetPPLevel( int level );

  private:
    theora_info      m_TheoraInfo;
    theora_comment   m_TheoraComment;
    theora_state     m_TheoraState;

    int m_MaxPostProcessLevel;
    int m_CurrentPostProcessLevel;

    enum STATE
    {
      STATE_EMPTY,  //  何もしてない
      STATE_INITIALIZING, //  初期化中
      STATE_DECODING,     //  デコード中
    };

    STATE m_State;

    double  m_DecodeStartTime;  //  デコードを開始する時間
    int     m_FrameCount;

    bool    m_IsSkipMode; //  スキップモードになった。
  };

}}

#endif