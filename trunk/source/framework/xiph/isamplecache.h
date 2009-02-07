#ifndef framework_xiph_isamplecache_h
#define framework_xiph_isamplecache_h

#include"../../config/define.h"
#include"../../auxiliary/thread.h"

#include"ibuffer.h"

#include <boost/smart_ptr.hpp>

namespace Maid { namespace Xiph {

  class ISampleCache
  {
  public:
    virtual ~ISampleCache(){}

    void Add( double begin, double end, const SPBUFFER& pBuffer );
    void Clear();


    //  溜まっているバッファから指定した時間以降のデータを抜き出す
    virtual void Flush( double TargetTime, SPBUFFER& Out )=0;

    //  キャッシュがいっぱいか？
    virtual bool IsFull() const=0;

  protected:
    struct SAMPLE
    {
      double BeginTime; //  開始時間（単位は秒）
      double EndTime;   //  終了時間（単位は秒）
      SPBUFFER pBuffer;

      //  BeginTime <= x < EndTime の範囲になる
    };
    typedef std::list<SAMPLE> SAMPLELIST;

    ThreadMutex m_Mutex;
    SAMPLELIST  m_List;
  };

  typedef boost::shared_ptr<ISampleCache> SPSAMPLECACHE;

}}
 
#endif