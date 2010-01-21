#include"filereaderall.h"
#include"storagemessage2.h"
#include"storagelocal.h"
#include"../auxiliary/debug/warning.h"

#include"../auxiliary/thread.h"

namespace Maid
{
  using namespace StorageMessage;

  //  このクラスはインスタンスがスレッドをまたぐと動作が不正になるので注意
  //  m_pBuffer の共有が正常に動かないため
  //  operator ==() 内でファイルを開き直せば問題なくなるかな？

  FileReaderAll::FileReaderAll()
  {

  }

  FileReaderAll::~FileReaderAll()
  {
    Close();
  }

  void FileReaderAll::Open( const String& filename )
  {
    Close();

    Storage* pStorage = GlobalPointer<Storage>::Get();
    SPSTORAGEOBJECT pObj = pStorage->CreateFileReader();
    SPMEMORYBUFFER  pBuff( new MemoryBuffer );

    boost::shared_ptr<FileROpenRead> pMess( new FileROpenRead );
    pMess->FileName = filename;
    pMess->pBuffer  = pBuff;
    pMess->pObject  = pObj;
    pStorage->SendMessage( pMess );

    m_pBuffer  = pBuff;
    m_pObject  = pObj;
    m_FileName = filename;
  }

  void FileReaderAll::Close()
  {
    //  一括読み込みが目的なので、Open() でファイルハンドルは自動的にクローズされます
    if( m_pObject.get()==NULL ) { return ; }
    m_pObject.reset();
    m_pBuffer.reset();
    m_FileName.clear();
  }

  bool    FileReaderAll::IsEmpty() const
  {
    return m_pObject.get()==NULL;
  }


  bool    FileReaderAll::IsExecuting() const
  {
    MAID_ASSERT( IsEmpty(), "まだOpenしてません" );
    if( IsEmpty() ) { return false; }
    return m_pObject->GetMessageCount()!=0;
  }

  bool    FileReaderAll::IsError() const
  {
    MAID_ASSERT( IsEmpty(), "まだOpenしてません" );
    if( IsEmpty() ) { return false; }
    return m_pObject->IsError();
  }

  size_t  FileReaderAll::GetSize() const
  {
    MAID_ASSERT( IsEmpty(), "まだOpenしてません" );
    if( IsEmpty() ) { return 0; }
    return m_pObject->GetSize();
  }

  const MemoryBuffer& FileReaderAll::GetBuffer() const
  {
    return *m_pBuffer;
  }

  const String& FileReaderAll::GetFileName() const
  {
    return m_FileName;
  }

  bool FileReaderAll::Sync()
  {
    while( true )
    {
      if( IsError() ) {  return true; }
      if( !IsExecuting() ) { break; }
      ThreadController::Sleep(1);
    }
    return false;
  }

}

