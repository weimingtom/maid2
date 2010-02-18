/*!
	@file
	@brief デコードの状態を管理するクラス ogg/ theora/vorbis
 */

#ifndef maid2_framework_movie_core_xiph_oggdecodermanager_h
#define maid2_framework_movie_core_xiph_oggdecodermanager_h


#include"../../../config/define.h"
#include"../idecodermanager.h"
#include"oggpage.h"
#include"oggstream.h"
#include"oggdecoder.h"

#include<map>

namespace Maid { namespace Movie { namespace Xiph {

  class OggDecoderManager
    : public IDecoderManager
  {
  public:
    OggDecoderManager();

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
      SPDECODER  pDecoder;
      SPSAMPLEFORMAT  pFormat;
      String     ID;
    };

    typedef std::map<int,INFO> DECODERLIST;
    DECODERLIST m_DecoderList;
    const INFO& FindInfo( const String& id ) const;
  };

}}}

#endif
