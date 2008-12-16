#include"filereader.h"
#include"storagemessage2.h"
#include"storagelocal.h"


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
    if( m_pObject.get()==NULL ) { return ; }

    boost::shared_ptr<FileRClose> pMess( new FileRClose );
    SendMessage( pMess );
    m_pObject.reset();
  }

  const FileReader::INFO& FileReader::GetStatus()const
  {
    INFO& info = const_cast<FileReader*>(this)->m_Info;

    if( m_pObject.get()==NULL )
    {
      info.Status = INFO::STATUS_SLEEPING;
    }else
    {
      if( m_pObject->GetMessageCount()==0 )  
      {
        StorageObjectFileReader* p = static_cast<StorageObjectFileReader*>(m_pObject.get());
        info.Size     = p->pReader->GetSize();
        info.Position = p->pReader->GetPosition();

        info.Status = INFO::STATUS_SLEEPING;  
      }
      else
      {
        info.Status = INFO::STATUS_READING;   
      }
    }

    return info;
  }

  void FileReader::CreateObject()
  {
    Close();
    m_Info.Status = INFO::STATUS_SLEEPING;
    Storage* p = GlobalPointer<Storage>::Get();
    m_pObject = p->CreateFileReader();
  }

  void FileReader::SendMessage( const SPSTORAGEMESSAGE& pMess )
  {
    pMess->pObject = m_pObject;
    GlobalPointer<Storage>::Get()->SendMessage( pMess );
  }

}

