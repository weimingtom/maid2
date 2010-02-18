/*!	
	@file
	@brief	ゲーム側から扱うストレージ
*/

#ifndef maid2_storage_storage_h
#define maid2_storage_storage_h

#include"../config/define.h"
#include"../config/typedef.h"
#include"../auxiliary/globalpointer.h"
#include"../auxiliary/thread.h"

#include"storagemessagebase.h"
#include"packfilereader.h"
#include"fileio/ifileread.h"
#include<map>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class Storage 
    : public GlobalPointer<Storage>
  {
  public:
    Storage();
    virtual ~Storage();

    void Initialize();
    void Initialize( unt32 ProcesserMask );
    void Finalize();

    SPSTORAGEOBJECT CreateFileWriter();
    SPSTORAGEOBJECT CreateFileReader();

    void SendMessage( const SPSTORAGEMESSAGE& pMess );

    bool IsFileExist( const String& FileName )const;

    PackFileReader& GetPackFileReader();

  private:
    void ExecuteMessage( const SPSTORAGEMESSAGE& pMess );

  private:  //  スレッド間をまたぐメンバ
    ThreadMutex         m_MessageMutex;
    STORAGEMESSAGEQUE   m_MessageQue;

  private:

    PackFileReader  m_PackFileReader;
    unt   m_SleepCount;

    SPFILEREAD OpenFileRead( const String& FileName );
    unt ThreadFunction(volatile ThreadController::BRIGEDATA& brige);
    ThreadController    m_AccessThread;
  };
}


#endif
