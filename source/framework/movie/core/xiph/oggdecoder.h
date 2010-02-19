#ifndef maid2_framework_movie_core_xiph_oggdecoder_h
#define maid2_framework_movie_core_xiph_oggdecoder_h

#include"../../../config/define.h"

#include"../idecoder.h"
#include"icodec.h"
#include"oggstream.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Movie { namespace Xiph {


  class OggDecoder
    : public IDecoder
  {
  public:
    OggDecoder( const SPOGGSTREAM& pStream, const SPCODEC& pCodec );

    virtual void Initialize();
    virtual void Finalize();

    virtual void AddSource( const SPSTORAGESAMPLE& buffer );

    virtual void FlushSample( double time, DECODERSAMPLELIST& pOut );
    virtual bool IsSourceFull() const;
    virtual bool IsSampleFull() const;
    virtual double GetTime() const;
    virtual void BeginSkipMode( double targettime );
    virtual bool IsDecodeEnd() const;


    bool IsPacketEmpty() const;

    void UpdateTime( DECODERSAMPLE& out );
    int  GetPacketCount() const;

    bool IsSkipMode() const;

  private:
    bool IsStreamEnd() const;

  private:
    SPOGGSTREAM m_pStream;
    SPCODEC     m_pCodec;
    double      m_SkipTargetTime; //  スキップし続ける時間
  };

  typedef boost::shared_ptr<OggDecoder> SPOGGDECODER;




}}}

#endif
