#ifndef framework_oggdecoder_h
#define framework_oggdecoder_h

#include"../config/define.h"
#include"../auxiliary/thread.h"

#include"xiph/oggstream.h"
#include"xiph/samplecache.h"
#include"xiph/idecoder.h"

#include<vector>

namespace Maid
{
  class ISampleCacheChecker
  {
  public:
    virtual ~ISampleCacheChecker(){}

    virtual bool IsFull( const Xiph::SampleCache& cache ) const=0;
  };

  typedef boost::shared_ptr<ISampleCacheChecker> SPSAMPLECACHECHECKER;


  class OggDecoder
  {
  public:
    OggDecoder();
    ~OggDecoder();

    void Initialize( const Xiph::SPOGGSTREAM& pStream, const Xiph::SPDECODER& pDecoder, const SPSAMPLECACHECHECKER& pChecker );
    void Finalize();
    void PageIn( const Xiph::OggPage& page );

    enum STATE
    {
      STATE_EMPTY,        //  初期化すらされていない
      STATE_DECOING,      //  デコード中
      STATE_PAGEWAIT,     //  新しいページがくるのを待っている（パケット切れ）
      STATE_CACHEFULL,    //  キャッシュが満タンなので待っている
      STATE_ENDOFSTREAM,  //  デコード終了
    };

    STATE GetState() const;
    void PopCache( double EndTime, Xiph::SAMPLELIST& Out );

  private:

    ThreadMutex m_StreamMutex;

    Xiph::SPOGGSTREAM  m_pStream;
    Xiph::SPDECODER    m_pDecoder;
    Xiph::SampleCache  m_Cache;

    SPSAMPLECACHECHECKER m_pChecker;

    bool m_IsNewPageIn; //  新しいページが挿入されたか？

    STATE m_State;

    std::vector<unsigned char>  m_PacketTmp;

  private:

    void OnStateDecoding();
    void OnStatePageWait();
    void OnStateCacheFull();
    void OnStateEndOfStream();

    unt ThreadFunction( volatile ThreadController::BRIGEDATA& state );
    ThreadController  m_Thread;
  };
}

#endif
