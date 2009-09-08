#include"filereaderall.h"
#include"storagemessage2.h"
#include"storagelocal.h"
#include"../auxiliary/debug/warning.h"

#include"../auxiliary/thread.h"

namespace Maid
{
  using namespace StorageMessage;


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
    m_pObject = pStorage->CreateFileReader();

    boost::shared_ptr<FileROpenRead> pMess( new FileROpenRead );
    pMess->FileName = filename;
    pMess->pBuffer  = &m_Buffer;
    pMess->pObject = m_pObject;
    pStorage->SendMessage( pMess );
  }

  void FileReaderAll::Close()
  {
    //  一括読み込みが目的なので、Open() でファイルハンドルはクローズされます
    if( m_pObject.get()==NULL ) { return ; }
    m_pObject.reset();
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
    return m_Buffer;
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

