/*!
	@file
	@brief デコードの状態を管理するクラス ogg/ theora/vorbis
 */

#ifndef maid2_framework_movie_core_xiph_oggdecodermanagersingle_h
#define maid2_framework_movie_core_xiph_oggdecodermanagersingle_h


#include"../../../config/define.h"
#include"../idecodermanager.h"
#include"oggpage.h"
#include"oggstream.h"
#include"oggdecoder.h"
#include"icachechecker.h"

#include<map>

namespace Maid { namespace Movie { namespace Xiph {

  class OggDecoderManagerSingle
    : public IDecoderManager
  {
  public:
    OggDecoderManagerSingle();

    virtual FUNCTIONRESULT Initialize();
    virtual bool Setuped()const;
    virtual void AddSource( const SPSTORAGESAMPLE& buffer );
    virtual void GetFormat( const String& id, SPSAMPLEFORMAT& pOut )const;
    virtual void FlushSample( const String& id, double time, DECODERSAMPLELIST& pOut );
    virtual double GetTime() const;
    virtual bool IsSourceFull() const;
    virtual bool IsSampleFull() const;

    virtual void BeginSkipMode( double time );
    virtual bool IsDecodeEnd() const;

    void Update();

  private:
    void AddDecoder( const OggPage& page );

    enum STATE
    {
      STATE_INITIALIZING, //  初期化中
      STATE_WORKING,      //  稼働中
    };

    STATE m_State;

  private:

    struct INFO
    {
      SPOGGDECODER  pDecoder;
      SPSAMPLEFORMAT  pFormat;
      DecoderSampleCache  Cache;
      SPCACHECHECKER pChecker;

      ThreadMutex DecodeMutex; //  m_Decoder の更新用
      ThreadMutex CacheMutex; //  m_Decoder の更新用

      String     ID;
    };

    bool IsSourceFull( const INFO& info ) const;
    bool IsCacheFull( const INFO& info ) const;

    typedef std::map<int,INFO> DECODERLIST;
    DECODERLIST m_DecoderList;
    const INFO& FindInfo( const String& id ) const;
    INFO& FindInfo( const String& id );
  };

}}}

#endif
