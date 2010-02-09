#ifndef maid2_framework_movie_core_storagereadermultithread_h
#define maid2_framework_movie_core_storagereadermultithread_h


#include"../../../config/define.h"
#include"../../../auxiliary/thread.h"
#include"istoragereader.h"

#include <list>



namespace Maid { namespace Movie { 

  class StorageReaderMultiThread
    : public IStorageReader
  {
  public:
    StorageReaderMultiThread( const SPSTORAGEREADER& pReader );

    virtual void Initialize();
    virtual void Finalize();

    virtual void Read( SPSTORAGESAMPLE& pSample );
    virtual bool IsEnd() const;
    virtual bool IsCacheEmpty() const;

  private:
    SPSTORAGEREADER m_pReader;

    std::list<SPSTORAGESAMPLE>  m_Cache;
    ThreadMutex m_CacheMutex;

  private:
    unt ThreadFunction( volatile ThreadController::BRIGEDATA& state );
    ThreadController  m_Thread;
  };


}}

#endif
