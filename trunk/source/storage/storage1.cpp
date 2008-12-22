#include"storage.h"

#include"storagelocal.h"

namespace Maid
{

Storage*  GlobalPointer<Storage>::s_pPointer;


Storage::Storage()
{
}

Storage::~Storage()
{
  Finalize();
  Clear();
}

void Storage::Initialize()
{
  GlobalPointer<Storage>::Set();
  m_AccessThread.SetFunc( MakeThreadObject(&Storage::ThreadFunction,this) );
  m_AccessThread.Execute();
}

void Storage::Finalize()
{
  m_AccessThread.Close();
}

SPSTORAGEOBJECT Storage::CreateFileWriter()
{
  return SPSTORAGEOBJECT( new StorageObjectFileWriter );
}

SPSTORAGEOBJECT Storage::CreateFileReader()
{
  return SPSTORAGEOBJECT( new StorageObjectFileReader );
}

void Storage::SendMessage( const SPSTORAGEMESSAGE& pMess )
{
  const unt id = ThreadController::GetCurrentID();
  if( id==m_AccessThread.GetID() )
  {
    //  同じスレッドだったら、その場で処理する。
    ExecuteMessage( pMess );
  }else
  {
    //  違う場合はキューにためておく
     ThreadMutexLocker lock(m_MessageMutex);
     m_MessageQue.push_back( pMess );

     pMess->pObject->m_MessageCount += 1;
  }
}

}
