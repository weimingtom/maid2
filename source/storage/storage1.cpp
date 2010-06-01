#include"storage.h"

#include"storagelocal.h"
#include"fileio/fileoperation.h"

namespace Maid
{

  /*!
      @class  Storage storage.h
      @brief  ストレージ管理クラス
  \n          ストレージとはオンメモリ以外のデータをさすものとします
  \n          ネットワークもストレージ
  */


Storage::Storage()
  :m_SleepCount(0)
{
}

Storage::~Storage()
{
  Finalize();
  Clear();
}


//! 初期化
/*!
 */
void Storage::Initialize()
{
  Initialize( 1 );
}

void Storage::Initialize( unt32 ProcesserMask )
{
  GlobalPointer<Storage>::Set();
  m_AccessThread.SetFunc( MakeThreadObject(&Storage::ThreadFunction,this) );
  m_AccessThread.Execute();
  m_AccessThread.SetProcesserMask( ProcesserMask );
}


//! 解放
/*!
 */
void Storage::Finalize()
{
  m_AccessThread.Close();
}

PackFileReader& Storage::GetPackFileReader()
{
  return m_PackFileReader;
}


//! 書き込みオブジェクトを発行する
/*!
 */
SPSTORAGEOBJECT Storage::CreateFileWriter()
{
  return SPSTORAGEOBJECT( new StorageObjectFileWriter );
}


//! 読み込みオブジェクトを発行する
/*!
 */
SPSTORAGEOBJECT Storage::CreateFileReader()
{
  return SPSTORAGEOBJECT( new StorageObjectFileReader );
}


//! オブジェクトにメッセージを送る
/*!
 */
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

//! ファイルが存在しているか調べる
/*!
    @param  FileName  [i ]  調べたいファイル名

    @return 存在しているなら true
            ないなら false
 */
bool Storage::IsFileExist( const String& FileName )const
{
  if ( FileOperation::IsFileExist(FileName) )
  {
    return true;
  }
  
  return m_PackFileReader.IsExist(FileName);
}


}
