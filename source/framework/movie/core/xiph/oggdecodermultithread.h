#ifndef maid2_framework_movie_core_xiph_oggdecodermultithread_h
#define maid2_framework_movie_core_xiph_oggdecodermultithread_h

#include"../../../../config/define.h"
#include"../../../../auxiliary/thread.h"

#include"oggdecoder.h"
#include"../decodersamplecache.h"
#include"icachechecker.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Movie { namespace Xiph {



  class OggDecoderMultiThread
    : public IDecoder
  {
  public:
    OggDecoderMultiThread( const SPOGGSTREAM& pStream, const SPCODEC& pCodec, const SPCACHECHECKER& pCheck, unt32 Mask );

    virtual void Initialize();
    virtual void Finalize();

    virtual void AddSource( const SPSTORAGESAMPLE& buffer );

    virtual void FlushSample( double time, DECODERSAMPLELIST& pOut );
    virtual bool IsSourceFull() const;
    virtual bool IsSampleFull() const;
    virtual bool IsDecodeEnd() const;

    virtual double GetTime() const;
    virtual void BeginSkipMode( double targettime );

  private:
    OggDecoder  m_Decoder;
    DecoderSampleCache  m_Cache;

    SPCACHECHECKER m_pChecker;

    ThreadMutex m_DecodeMutex; //  m_Decoder の更新用
    ThreadMutex m_CacheMutex; //  m_Decoder の更新用

    bool  m_IsSourceFull;
    bool  m_IsSampleFull;
    bool  m_IsDecodeEnd;

    const unt32 m_ThreadMask; //  処理スレッドを固定するかのマスク(0で固定しない)

  private:
    unt ThreadFunction( volatile ThreadController::BRIGEDATA& state );
    ThreadController  m_Thread;
  };

  typedef boost::shared_ptr<OggDecoderMultiThread> SPOGGDECODERMULTITHREAD;

}}}

#endif
