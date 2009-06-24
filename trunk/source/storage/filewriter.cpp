#include"filewriter.h"
#include"storagemessage1.h"
#include"../auxiliary/thread.h"

#include"../auxiliary/debug/warning.h"

namespace Maid
{
  using namespace StorageMessage;

  /*!
      @class  FileWriter filewriter.h
      @brief  非同期ファイル書き込みクラス
  */

  FileWriter::FileWriter()
  //  :m_pObject( new IStorageObject )
  {
  }

  FileWriter::~FileWriter()
  {
    Close();
  }

  void FileWriter::Create( const String& filename )
  {
    CreateObject();

    boost::shared_ptr<FileWCreate> pMess( new FileWCreate );
    pMess->FileName = filename;

    SendMessage( pMess );
  }

  void FileWriter::Open( const String& filename )
  {
    CreateObject();

    boost::shared_ptr<FileWOpen> pMess( new FileWOpen );
    pMess->FileName = filename;

    SendMessage( pMess );
  }

  void FileWriter::OpenAdd( const String& filename )
  {
    CreateObject();

    boost::shared_ptr<FileWAdd> pMess( new FileWAdd );
    pMess->FileName = filename;

    SendMessage( pMess );
  }

  void FileWriter::Write( const void* pData, size_t size )
  {
    if( size==0 ) { return ; }
    boost::shared_ptr<FileWWrite> pMess( new FileWWrite );
    pMess->Buffer.resize( size );
    
    memcpy( &(pMess->Buffer[0]), pData, size );

    SendMessage( pMess );
  }

  void FileWriter::Seek( POSITION Pos, int Size )
  {
    boost::shared_ptr<FileWSeek> pMess( new FileWSeek );
    pMess->Size = Size;

    switch( Pos )
    {
    case POSITION_BEGIN:    { pMess->Pos = FileWSeek::POSITION_BEGIN;   }break;
    case POSITION_END:      { pMess->Pos = FileWSeek::POSITION_END;     }break;
    case POSITION_CURRENT:  { pMess->Pos = FileWSeek::POSITION_CURRENT; }break;
    }

    SendMessage( pMess );
  }

  void FileWriter::Close()
  {
    Close( false );
  }

  void FileWriter::Close( bool IsSync )
  {
    if( m_pObject.get()==NULL ) { return ; }

    boost::shared_ptr<FileWClose> pMess( new FileWClose );
    SendMessage( pMess );

    if( IsSync )
    {
      while( true )
      {
        if( IsError() ) { MAID_WARNING( MAIDTEXT("FileWriter::Close()") ); break; }
        if( !IsExecuting() ) { break; }
        ThreadController::Sleep(1);
      }
    }

    m_pObject.reset();
  }

  bool FileWriter::IsEmpty() const
  {
    return m_pObject.get()==NULL;
  }

  bool FileWriter::IsExecuting() const
  {
    MAID_ASSERT( IsEmpty(), "まだOpenしてません" );
    if( IsEmpty() ) { return false; }
    return m_pObject->GetMessageCount()!=0;
  }

  bool FileWriter::IsError() const
  {
    MAID_ASSERT( IsEmpty(), "まだOpenしてません" );
    if( IsEmpty() ) { return false; }
    return m_pObject->IsError();
  }

  size_t  FileWriter::GetSize() const
  {
    MAID_ASSERT( IsEmpty(), "まだOpenしてません" );
    if( IsEmpty() ) { return 0; }
    return m_pObject->GetSize();
  }

  size_t  FileWriter::GetPosition() const
  {
    MAID_ASSERT( IsEmpty(), "まだOpenしてません" );
    if( IsEmpty() ) { return 0; }
    return m_pObject->GetPosition();
  }

  void FileWriter::CreateObject()
  {
    Close();
    Storage* p = GlobalPointer<Storage>::Get();
    m_pObject = p->CreateFileWriter();
  }

  void FileWriter::SendMessage( const SPSTORAGEMESSAGE& pMess )
  {
    pMess->pObject = m_pObject;
    GlobalPointer<Storage>::Get()->SendMessage( pMess );
  }

}

