#include"filereader.h"
#include"storagemessage2.h"
#include"storagelocal.h"
#include"../auxiliary/debug/warning.h"

#include"../auxiliary/thread.h"

namespace Maid
{
  using namespace StorageMessage;

  FileReader::FileReader()
  {
  }

  FileReader::~FileReader()
  {
    Close();
  }

  void FileReader::Open( const String& filename )
  {
    CreateObject();

    boost::shared_ptr<FileROpen> pMess( new FileROpen );
    pMess->FileName = filename;

    SendMessage( pMess );
  }

  void FileReader::Read( void* pData, size_t size )
  {
    boost::shared_ptr<FileRRead> pMess( new FileRRead );
    pMess->pDst = pData;
    pMess->Size = size;

    SendMessage( pMess );
  }

  void FileReader::Seek( POSITION Pos, int Size )
  {
    boost::shared_ptr<FileRSeek> pMess( new FileRSeek );
    pMess->Size = Size;

    switch( Pos )
    {
    case POSITION_BEGIN:    { pMess->Pos = FileRSeek::POSITION_BEGIN;   }break;
    case POSITION_END:      { pMess->Pos = FileRSeek::POSITION_END;     }break;
    case POSITION_CURRENT:  { pMess->Pos = FileRSeek::POSITION_CURRENT; }break;
    }

    SendMessage( pMess );
  }

  void FileReader::Close()
  {
    Close( false );
  }

  void FileReader::Close( bool IsSync )
  {
    if( m_pObject.get()==NULL ) { return ; }

    boost::shared_ptr<FileRClose> pMess( new FileRClose );
    SendMessage( pMess );

    if( IsSync )
    {
      if( Sync() )
      {
        MAID_WARNING( MAIDTEXT("FileReader::Close()") ); 
      }
    }

    m_pObject.reset();
  }

  bool FileReader::Sync()
  {
    while( true )
    {
      if( IsError() ) {  return false; }
      if( !IsExecuting() ) { break; }
      ThreadController::Sleep(1);
    }
    return true;
  }


  bool FileReader::IsEmpty() const
  {
    return m_pObject.get()==NULL;
  }

  bool FileReader::IsExecuting() const
  {
    MAID_ASSERT( IsEmpty(), "まだOpenしてません" );
    if( IsEmpty() ) { return false; }
    return m_pObject->GetMessageCount()!=0;
  }

  bool FileReader::IsError() const
  {
    MAID_ASSERT( IsEmpty(), "まだOpenしてません" );
    if( IsEmpty() ) { return false; }
    return m_pObject->IsError();
  }

  size_t  FileReader::GetSize() const
  {
    MAID_ASSERT( IsEmpty(), "まだOpenしてません" );
    if( IsEmpty() ) { return 0; }
    return m_pObject->GetSize();
  }

  size_t  FileReader::GetPosition() const
  {
    MAID_ASSERT( IsEmpty(), "まだOpenしてません" );
    if( IsEmpty() ) { return 0; }
    return m_pObject->GetPosition();
  }


  void FileReader::CreateObject()
  {
    Close();
    Storage* p = GlobalPointer<Storage>::Get();
    m_pObject = p->CreateFileReader();
  }

  void FileReader::SendMessage( const SPSTORAGEMESSAGE& pMess )
  {
    pMess->pObject = m_pObject;
    GlobalPointer<Storage>::Get()->SendMessage( pMess );
  }

}

